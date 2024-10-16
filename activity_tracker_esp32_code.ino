#include <Wire.h>
#include <MPU6050.h>
#include "display.h"
#include <sim.h>
#include <ESP32Time.h>
#include <esp_sleep.h>
#include "server.h"
#include "driver/adc.h"

//ESP32Time rtc;
// ESP32Time rtc(0);  // offset in seconds GMT+1

// const int MPU_addr = 0x68; // I2C address of the MPU-6050
// int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
// float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;
boolean fall = false;     // stores if a fall has occurred
boolean trigger1 = false; // stores if first trigger (lower threshold) has occurred
boolean trigger2 = false; // stores if second trigger (upper threshold) has occurred
boolean trigger3 = false; // stores if third trigger (orientation change) has occurred
byte trigger1count = 0;   // stores the counts past since trigger 1 was set true
byte trigger2count = 0;   // stores the counts past since trigger 2 was set true
byte trigger3count = 0;   // stores the counts past since trigger 3 was set true
int angleChange = 0;
uint8_t catchActivity = 0;

uint32_t prevRunCheck = 0;
uint8_t runCheckAddr = 0;


// #define INTERRUPT_PIN 5
#define SDA 6
#define SCL 7
#define ACCL_PIN 8
#define POWER_PIN 4
#define SIM_POWER 10

#define NORMAL 0
#define SETUP 1
#define POWER_OFF 2
#define POWER_ON 3

uint32_t timeout = millis();
uint32_t activityTimeout = millis();
uint32_t displayTimeout = millis();
uint32_t powerSwitchTimeOut = millis();
bool endSimFlag = false;
bool setupmode = false;
uint8_t retries = 0;
uint8_t switchState = 0;

MPU6050 imu;
volatile bool falldetection = false;

int count = 0;
int16_t ax, ay, az;
int16_t gx, gy, gz;
float gxx, gyy, gzz;
float accX, accY, accZ;

bool blinkState = false;
bool walkCapture = false;
bool uploadFlag = false;
bool tapDetected = false;
uint8_t tapCount = 0;
uint8_t formerTap = 0;
uint8_t tapValue = 0;
uint32_t captureFormerTapMillis, tapTimeOut = 0;
uint32_t tapTime, dtapTime = 0, dtapApprove;

const float fallThreshold = 900000; // Adjust this value to suit your needs (in m/s^3)
const float tapThreshold = 20;
const float walkThreshold = 6;
const float runThreshold = 30;
unsigned long runCheck = 0;
uint32_t lastIdleTime, lastActiveTime = 0;
RTC_DATA_ATTR unsigned long idleTime = 0;
RTC_DATA_ATTR unsigned long activeTime = 0;

const int sampleInterval = 20;   // Interval in milliseconds between readings
float prevAccX = 0.0, prevAccY = 0.0, prevAccZ = 0.0;

uint16_t waitforValidWalk = 0;
RTC_DATA_ATTR unsigned long stepCount = 0;
uint8_t checkActivityChange = 0;
bool powerSwitchFlag = false;
uint8_t deviceState = NORMAL;

adc1_channel_t channel = ADC1_CHANNEL_0;
adc_atten_t atten = ADC_ATTEN_DB_11;

uint32_t dispWalkTimer = millis();

#define DEFAULT_SOS "09059571806"
#define DEFAULT_APN "gloflat"
#define DEFAULT_NAME "User"

uint32_t powerPinDebounce = millis();
uint8_t optionSelected;
bool optionMode =false;

bool IRAM_ATTR do_not_upload = false;

void IRAM_ATTR power_ISR() {
    powerSwitchFlag = true;
}

String sos, networkAPN;
void initMemory(){
 sos = readFile(sos1Path);
 if(sos==""){
  sos = DEFAULT_SOS;
 } 

String net = readFile(apnPath);
 if(net==""){
  networkAPN = DEFAULT_APN;
 } else {
  networkAPN = makeAPN(net);
 }
}


void blink(uint8_t pin, uint8_t t){
  for(int a=0;a<t;a++){
 digitalWrite(pin, LOW);
 _delay(200);
 digitalWrite(pin, HIGH);
 _delay(200);
  }
}

float magnitude(float x,float y, float z){
 return sqrt((x*x)+(y*y)+(z*z));
}

float partialDiff;
bool diffCount = false;
uint32_t diffTime = 0;

float checkDiff(float num)
{
  float diff = 1000.0;
  if (!diffCount)
  {
    partialDiff = num;
    diffTime = millis();
    diffCount = true;
  }
  else
  {
    if (millis() - diffTime > 5000)
    {
      diff = num - partialDiff;
      diffTime = millis();
      diffCount = false;
    }
  }
  return diff;
}

void _delay(uint32_t d){
  uint32_t t = millis();
  while((millis()-t)<d);
}

uint8_t walkAlgorithm();
bool fallAlgorithm(int Amp);

bool simComms()
{
  bool continueProcess = true;
  retries += 1;
  if (beginSIM(true))
  {
    int cnt = 0;
    uint8_t net = 0;
    uint8_t gsmOK = 1;
    while (1)
    {
      net = getNetworkStatus();
      _delay(1000);
      Serial.printf("Retried No: %d, Status: %d\n",cnt,net);
      cnt++;
      if (cnt> 20 || net == 1 ) {
        break;
      }
      }

    if(cnt>=20){
      continueProcess = false;
    }
    else
    {
      cnt = 0;
      bool gprsOn = false;
      while (!gprsOn)
      {
        gprsOn = enableGPRS(true, networkAPN);
        Serial.println("GPRS fail");
        _delay(1000);
        cnt++;
        Serial.printf("GPRS Retried No: %d", cnt);
        if (cnt >= 20 || powerSwitchFlag)
        {
          continueProcess = false;
          cnt = 0;
          break;
        }
      }
    }
  }
  return continueProcess;
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin(SDA,SCL);
  Serial.begin(115200);

  pinMode(POWER_PIN, INPUT_PULLUP);

  esp_deep_sleep_enable_gpio_wakeup(0x10,ESP_GPIO_WAKEUP_GPIO_LOW);

  if(!digitalRead(POWER_PIN)){
                                  _delay(3000);
                                  if(!digitalRead(POWER_PIN)){
                                    Serial.println("Device power up");
                                  } else {
                                    Serial.println("Go back to deep sleep");
                                    esp_deep_sleep_start();
                                  }
                                 }
  
  pinMode(ACCL_PIN, OUTPUT);
  

  _delay(1);
  digitalWrite(ACCL_PIN, LOW);
  Serial.println("Device Started");
  _delay(100);

   imu.initialize();
  beginDisplay();
  while(!digitalRead(POWER_PIN));

  Serial.println(imu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(channel, atten);
    
  attachInterrupt(digitalPinToInterrupt(POWER_PIN),power_ISR,FALLING);
  rtc.setTime(0, 15, 12, 28, 7, 2024);
  beginFS();
  delay(1);
  initMemory();
}

void loop() {
//check battery level 
  if(batLevel()<10){
    Serial.println("battery Low");
    delay(1000);
    // digitalWrite(ACCL_PIN, HIGH);
    // beginSIM(false);
    // dispShutDown();

    // pinMode(ACCL_PIN, INPUT);
    // pinMode(SIM_POWER, INPUT);
    // pinMode(SCREEN_POWER, INPUT);
    // esp_deep_sleep_start();
  }

 imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    accX = (ax - 493.81) / 16384.00;
    accY = (ay ) / 16384.00;
    accZ = (az + 3481.6)  / 16384.00;

    gxx = gx / 131.07;
    gyy = gy / 131.07;
    gzz = gz / 131.07;

    float accMagnitude = magnitude(accX, accY, accZ)*10;
    float gyroMagnitude = magnitude(gxx, gyy, gzz);
    // Serial.printf("aX=%.1f,aY=%.1f,aZ=%.1f,aXYZ=%.1f,\n",accX, accY, accZ, accMagnitude);
// Serial.printf("%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,\n",accX, accY, accZ, accMagnitude,gxx,gyy,gzz,gyroMagnitude);

    // Calculate jerk by taking the derivative of acceleration with respect to time
    float jerkX = (accX - prevAccX) / (sampleInterval / 1000.0);
    float jerkY = (accY - prevAccY) / (sampleInterval / 1000.0);
    float jerkZ = (accZ - prevAccZ) / (sampleInterval / 1000.0);

    // Update previous acceleration values for the next iteration
    float prevMagnitude = magnitude(prevAccX, prevAccY, prevAccZ)*10;
  
    prevAccX = accX;
    prevAccY = accY;
    prevAccZ =accZ;

    // Calculate the magnitude of jerk
    float jerkMagnitude = magnitude(jerkX, jerkY, jerkZ);
    // Serial.println(jerkMagnitude);
    
    // Check for a fall, or walking
    if(jerkMagnitude>tapThreshold && !trigger1 && !trigger2){
        // if(accY>=-0.2 && accY<=0.2){
      tapDetected = true;
      displayON(true);
      displayTimeout = millis();
        // }
    } else if(fallAlgorithm((int)accMagnitude)){
      Serial.println("Fall Detected!");
      char* msg = "Fall detected. Kindly confirm";
      drawActivity(0,90,catchActivity);
      displayON(true);
      displayTimeout = millis();
      catchActivity = 10;
      } else if(checkDiff(accMagnitude)<0.3){
          // Serial.println("IDLE");
          catchActivity = 1;
      } else {
      uint8_t steps = walkAlgorithm();
      switch(steps) {
        case 0: // no step
        break;

        case 1: 
        catchActivity = 2;
        stepCount++;
        Serial.printf("WALKING, steps: %d\n", stepCount);
        break;
      }
    } 

    if(checkActivityChange != catchActivity) {
      retries = 0;
      checkActivityChange = catchActivity;
      if(catchActivity == 1){
        lastIdleTime = millis();
      } else {
        idleTime += millis() - lastIdleTime;
      }
      Serial.printf("Idle Time: %d secs\n", idleTime/1000);
    }

    if(catchActivity == 10){
      // checkActivityChange = catchActivity;
      activityTimeout = millis();
      // drawWait(0,90);
      retries = 0;
      endSimFlag = false;
      if(simComms()){
        sendMessage(sos,"Fall detected, Please confirm with user");
        singleUpdate();
      }
    }

    if(millis()-activityTimeout >= 60000 && !endSimFlag) {
      drawWait(0,90);
      // endSIM();
      beginSIM(false);
      endSimFlag = true;
    }

    if(millis() - displayTimeout >= 20000){
      displayON(false);
      displayTimeout = millis();
      clearDisplay();
    }

/*********************************** */
    if(millis() - powerSwitchTimeOut >= 5000 && !setupmode){
      deviceState = 0;
      optionSelected = 0;
      switchState = 0;
      powerSwitchTimeOut = millis();
      attachInterrupt(digitalPinToInterrupt(POWER_PIN),power_ISR,FALLING);
    }

    if(powerSwitchFlag){
      while(!digitalRead(POWER_PIN));
      powerSwitchTimeOut = millis();
      displayTimeout = millis();
      displayON(true);
      _delay(300);
      switchState++;
      constrain(switchState,0,2);
      Serial.printf("Switch state %s\n",switchState==1?"OPTION":switchState==2?"SELECT":"NONE");
      
      if(switchState ==2) {
        optionSelected = deviceState;
        Serial.printf("Option: %d",optionSelected);
        switch(optionSelected){
          case 0: displaySetup(true);
                  Serial.println("Entering Setup");
                  setupmode = true;
                  detachInterrupt(POWER_PIN);
                  runWebServer();
                  while(!settingsAbort()){
                    // delay(1000);
                    if(!digitalRead(POWER_PIN)){
                      delay(300);
                      if(!digitalRead(POWER_PIN)){
                        deviceState++;
                      }
                    }
                    if(deviceState>=4) break;
                    }
                  deviceState = 0;
                  optionSelected = 0;
                  displayTimeout = millis();
                  clearDisplay();
                  attachInterrupt(digitalPinToInterrupt(POWER_PIN),power_ISR, FALLING);
                  setupmode = false;
                  endServer();
                  break;

          case 1: Serial.println("Entering Upload");
                  displayUpload(true);
                  if(retries<2 && simComms()){
                  singleUpdate(idleTime, stepCount);
                  }
                  deviceState = 0;
                  optionSelected = 0;
                  displayTimeout = millis();
                  clearDisplay();
                  break;
          case 2: displayPower(true);
                  Serial.println("Powering off");
                  delay(3000);
                  digitalWrite(ACCL_PIN, HIGH);
                  beginSIM(false);
                  dispShutDown();

                  pinMode(ACCL_PIN, INPUT);
                  pinMode(SIM_POWER, INPUT);
                  pinMode(SCREEN_POWER, INPUT);
                  esp_deep_sleep_start();
                  break;
          }
       }
      
      powerSwitchTimeOut = millis();
      powerSwitchFlag = false;       
    }  

//*****************************************************
    if(switchState == 1){
      if(tapDetected){
        tapDetected = false;
        delay(300);
        deviceState++;
          if(deviceState>3){
            deviceState = 0;
          }
        powerSwitchTimeOut = millis();
        displayTimeout = millis();
      }

      switch(deviceState){
                    case 0: displaySetup(false);
                            Serial.println("Displaying Setup Mode");
                            break;
                    case 1: displayUpload(false);
                            retries = 0;
                            break;
                    case 2: displayPower(false);
                            Serial.println("Displaying Power off");
                            break;
                            }
      }


  if(((millis()-dispWalkTimer) > 1000) && switchState == 0) {
    dispWalkTimer = millis();
    drawBatteryLevel(batLevel(),0,0);
    drawDateTime(0,40, rtc.getHour(), rtc.getMinute(), rtc.getAmPm(false), rtc.getDate()); 
    drawSteps(0,110,stepCount);
    drawActivity(0,90,catchActivity);
  }

  if(rtc.getHour() == 0){
    if(!uploadFlag) {
      Serial.println("timer based upload");
      uploadFlag = true;
      displayUpload(true);
    if(retries<1 && simComms()){
      singleUpdate(idleTime, stepCount);
      idleTime = 0;
      stepCount = 0;
    }
    }
  } else {
    uploadFlag = false;
  }

  _delay(10);

}

/**** 
void tapOption(){
  uint32_t t = millis();
      powerSwitchTimeOut = millis();
      displayTimeout = millis();
      displayON(true);
      if(millis()-tapTime > 200){
        tapTime = millis();
        tapCount++;
        // Serial.println("\nTap detected");

        if(tapCount == 1){
        dtapTime = millis();
        dtapApprove = millis();
        tapValue = 1;
        Serial.println("Single tap");
      } 
      
      if(tapCount >= 2){
        
        uint32_t t = millis() - dtapTime;
        Serial.printf("Tap ms: %d", t);
        tapCount = 0;
        if(t>100 && t<400){
          dtapTime = millis();
          Serial.println("double tap");
          tapValue = 2;
        } else {
          tapValue = 1;
          
          tapCount = 0;
        }
      }  
      }  

         
          
      powerSwitchTimeOut = millis();
      if(optionMode) {
      if(optionSelected == 0) deviceState++;
      deviceState = constrain(deviceState,0,3);


      switch(optionSelected){
        case 0: switch(deviceState){
                    case 1: displaySetup(false);
                            Serial.println("Displaying Setup Mode");
                            break;
                    case 2: displayUpload(false);
                            retries = 0;
                            break;
                    case 3: displayPower(false);
                            Serial.println("Displaying Power off");
                            break;
                            }
                break;
        case 1: displaySetup(true);
                Serial.println("Entering Setup");
                setupmode = true;
                runWebServer();
                while(!settingsAbort()){
                  // delay(1000);
                  if(!digitalRead(POWER_PIN)){
                    delay(300);
                    if(!digitalRead(POWER_PIN)){
                      deviceState++;
                    }
                  }
                  if(deviceState>=4) break;
                  }
                deviceState = 0;
                optionSelected = 0;
                displayTimeout = millis();
                clearDisplay();
                
                setupmode = false;
                endServer();
                
                break;
        case 2: 
                displayUpload(true);
                if(retries<2 && simComms()){
                singleUpdate(idleTime, stepCount);
                }
                deviceState = 0;
                optionSelected = 0;
                displayTimeout = millis();
                clearDisplay();
                break;
        case 3: displayPower(true);
                Serial.println("Powering off");
                delay(3000);
                digitalWrite(ACCL_PIN, HIGH);
                beginSIM(false);
                dispShutDown();

                pinMode(ACCL_PIN, INPUT);
                pinMode(SIM_POWER, INPUT);
                pinMode(SCREEN_POWER, INPUT);
                esp_deep_sleep_start();
                break;
      }


      }
      

      // Serial.printf("Power Switch Option, %d\n",optionSelected);
      // Serial.printf("Power Button State, %d\n",deviceState);
      // tapDetected = false;
}

****/
bool fallAlgorithm(int Amp){
  bool f = false;

   if (Amp <= 2){
    _delay(100);
   }
  if (Amp <= 2 && trigger2 == false)
  { // if AM breaks lower threshold (0.4g)
    trigger1 = true;
    // Serial.println("TRIGGER 1 ACTIVATED");
  }

  if (trigger1 == true)
  {
    trigger1count++;
    if (Amp >= 12)
    { // if AM breaks upper threshold (3g)
    
    // Serial.printf("Impact Force: %d\n", Amp);
      trigger2 = true;
      // Serial.println("TRIGGER 2 ACTIVATED");
      trigger1 = false;
      trigger1count = 0;
    }
  }

  if (trigger2 == true)
  {
    trigger2count++;
    angleChange = pow(pow(gxx, 2) + pow(gyy, 2) + pow(gzz, 2), 0.5);
    // Serial.println(angleChange);
    if (angleChange >= 30 && angleChange <= 400)
    { // if orientation changes by between 80-100 degrees
      trigger3 = true;
      trigger2 = false;
      trigger2count = 0;
      // Serial.println(angleChange);
      // Serial.println("TRIGGER 3 ACTIVATED");
    }
  }

  if (trigger3 == true)
  {
    trigger3count++;
    if (trigger3count >= 100)
    {
      angleChange = pow(pow(gxx, 2) + pow(gyy, 2) + pow(gzz, 2), 0.5);
      // _delay(10);
      // Serial.println(angleChange);
      if ((angleChange >= 0) && (angleChange <= 10))
      { // if orientation changes remains between 0-10 degrees
        f = true;
        trigger3 = false;
        trigger3count = 0;
        // Serial.println(angleChange);
      }
      else
      { // user regained normal orientation
        trigger3 = false;
        trigger3count = 0;
        // Serial.println("TRIGGER 3 DEACTIVATED");
      }
    }
  }

  if (trigger2count >= 6)
  { // allow 0.5s for orientation change
    trigger2 = false;
    trigger2count = 0;
    //Serial.println("TRIGGER 2 DECACTIVATED");
  }

  if (trigger1count >= 6)
  { // allow 0.5s for AM to break upper threshold
    trigger1 = false;
    trigger1count = 0;
    //Serial.println("TRIGGER 1 DECACTIVATED");
  }

  return f;
}

uint8_t walkAlgorithm(){
  uint8_t t = 0;
  
  if(accZ>0.1 && accZ<=0.6) {
    if(waitforValidWalk<55){
    waitforValidWalk++;
    // Serial.printf("Walk mode trigger - %d\n", waitforValidWalk);
    }
  } else waitforValidWalk = 0;

  if(waitforValidWalk>=50){
  float absY = abs(accY);
  float absG = abs(gzz);
  
  if(absY >=1.0 && absG>=40 && !walkCapture){
    // Serial.printf("Walk mode - %.1f, %.1f\n", absY, gzz);
    t = 1;
    walkCapture = true;
  }else {
    t = 0;
    // walkCapture = false;
  }
  if(absG<10) walkCapture = false;
  }
  return t;
}

uint8_t batLevel(){
  // long v = adc1_get_raw(channel);
  long v = analogRead(0);
  // Serial.printf("Raw Int: %d\n", v);
  Serial.printf("Raw Batt Volt: %.2f\n", v*0.0015024);

  long j = map(v,2063,2795,0,100);
  j = constrain(j,0,100);

  Serial.printf("Batt Percentage: %d\n", j);

  return j;
}


