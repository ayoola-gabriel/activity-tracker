#line 1 "C:\\Users\\Ayoola\\Documents\\Newness Files\\projects\\Activity Tracker\\activity_tracker_esp32_code\\display.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "robotbold10.h"
#include <fonts/Org_01.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_POWER 2

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 'activity', 32x32px
const unsigned char bitmap_activity [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 
	0x00, 0x1e, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 
	0x00, 0x3e, 0x00, 0x00, 0x03, 0x3f, 0x1c, 0x00, 0x07, 0xbf, 0x1c, 0x00, 0x07, 0xf7, 0x1e, 0x00, 
	0xff, 0xf7, 0x3e, 0x7f, 0xff, 0xf3, 0x3f, 0xff, 0x7c, 0xf3, 0x37, 0xfe, 0x00, 0xe3, 0xf7, 0xc0, 
	0x00, 0xe3, 0xf3, 0x80, 0x00, 0x63, 0xe3, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xc0, 0x00, 
	0x00, 0x01, 0xc0, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmap_settings [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 
	0x00, 0x07, 0xe0, 0x00, 0x00, 0x0e, 0x70, 0x00, 0x00, 0x1e, 0x78, 0x00, 0x03, 0xbe, 0x7d, 0xc0, 
	0x07, 0xf8, 0x1f, 0xe0, 0x0f, 0xe0, 0x07, 0xf0, 0x0e, 0x40, 0x02, 0x70, 0x1e, 0x07, 0xe0, 0x78, 
	0x1f, 0x0f, 0xf0, 0xf8, 0x07, 0x1e, 0x78, 0xe0, 0x07, 0x1c, 0x38, 0xe0, 0x07, 0x18, 0x18, 0xe0, 
	0x07, 0x18, 0x18, 0xe0, 0x07, 0x1c, 0x38, 0xe0, 0x07, 0x1e, 0x78, 0xe0, 0x1f, 0x0f, 0xf0, 0xf8, 
	0x1e, 0x07, 0xe0, 0x78, 0x0e, 0x40, 0x02, 0x70, 0x0f, 0xe0, 0x07, 0xf0, 0x07, 0xf8, 0x1f, 0xe0, 
	0x03, 0xbe, 0x7d, 0xc0, 0x00, 0x1e, 0x78, 0x00, 0x00, 0x0e, 0x70, 0x00, 0x00, 0x07, 0xe0, 0x00, 
	0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmap_shutdown [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 
	0x00, 0x3c, 0x3c, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x01, 0x80, 0x01, 0x80, 0x03, 0x01, 0x80, 0xc0, 
	0x06, 0x01, 0x80, 0x60, 0x04, 0x19, 0x98, 0x20, 0x0c, 0x31, 0x8c, 0x30, 0x08, 0x61, 0x86, 0x10, 
	0x18, 0x41, 0x82, 0x18, 0x18, 0xc0, 0x03, 0x18, 0x10, 0x80, 0x01, 0x08, 0x10, 0x80, 0x01, 0x08, 
	0x10, 0x80, 0x01, 0x08, 0x10, 0x80, 0x01, 0x08, 0x18, 0xc0, 0x03, 0x18, 0x18, 0x40, 0x02, 0x18, 
	0x08, 0x60, 0x06, 0x10, 0x0c, 0x30, 0x0c, 0x30, 0x04, 0x1c, 0x38, 0x20, 0x06, 0x07, 0xe0, 0x60, 
	0x03, 0x00, 0x00, 0xc0, 0x01, 0x80, 0x01, 0x80, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x3c, 0x3c, 0x00, 
	0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void beginDisplay(){
pinMode(SCREEN_POWER, OUTPUT);
int ret=gpio_set_drive_capability((gpio_num_t)2, GPIO_DRIVE_CAP_3);
// Serial.print("set_drive_cap returns "); Serial.println(ret);
digitalWrite(SCREEN_POWER, HIGH);
delay(10);

Serial.println("Screen powered");

if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  delay(1);
  display.setRotation(3);
  display.drawBitmap(0,48,bitmap_activity, 32, 32, SSD1306_WHITE);


  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(5000); // Pause for 2 seconds
  display.clearDisplay();
  }


void displayON(bool s)
{
  if (s)
  {
    display.ssd1306_command(SSD1306_DISPLAYON);
  }
  else
  {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }
}

void dispShutDown(){
  displayON(false);
  delay(1);
  digitalWrite(SCREEN_POWER, LOW);
}

void drawRSSI(uint8_t rssi, int16_t x, int16_t y)
{

  // display.drawTriangle(0, 0, 3, 3, 6, 0, WHITE);
  display.drawTriangle(x, y, x + 3, y + 3, x + 6, y, WHITE);
  display.drawLine(x + 3, y, x + 3, y + 8, WHITE);
  if ((rssi >= -110) && (rssi < -105))
  {
    display.drawRect(x + 6, y + 7, 2, 2, WHITE);
  }
  else if ((rssi >= -105) && (rssi < -97))
  {
    display.drawRect(x + 6, y + 7, 2, 2, WHITE);
    display.drawRect(x + 10, y + 5, 2, 4, WHITE);
  }
  else if ((rssi >= -97) && (rssi < -90))
  {
    display.drawRect(x + 6, y + 7, 2, 2, WHITE);
    display.drawRect(x + 10, y + 5, 2, 4, WHITE);
    display.drawRect(x + 14, y + 3, 2, 6, WHITE);
  }
  else if (rssi >= -90)
  {
    display.drawRect(x + 6, y + 7, 2, 2, WHITE);
    display.drawRect(x + 10, y + 5, 2, 4, WHITE);
    display.drawRect(x + 14, y + 3, 2, 6, WHITE);
    display.drawRect(x + 18, y + 1, 2, 8, WHITE);
  }
}

#define bitmap_bat_height 5
#define bitmap_bat_width 22

static const unsigned char PROGMEM bitmap_bat100[] = {
 B00111111, B11111111, B11111100,
 B10100000, B00000000, B00000100,
 B10101111, B01110111, B01110100,
 B10100000, B00000000, B00000100,
 B00111111, B11111111, B11111100,
};

static const unsigned char PROGMEM bitmap_bat75[] = {
 B00111111, B11111111, B11111100,
 B10100000, B00000000, B00000100,
 B10100000, B01110111, B01110100,
 B10100000, B00000000, B00000100,
 B00111111, B11111111, B11111100,
};

static const unsigned char PROGMEM bitmap_bat50[] = {
 B00111111, B11111111, B11111100,
 B10100000, B00000000, B00000100,
 B10100000, B00000111, B01110100,
 B10100000, B00000000, B00000100,
 B00111111, B11111111, B11111100,
};

static const unsigned char PROGMEM bitmap_bat25[] = {
 B00111111, B11111111, B11111100,
 B10100000, B00000000, B00000100,
 B10100000, B00000000, B01110100,
 B10100000, B00000000, B00000100,
 B00111111, B11111111, B11111100,
};

static const unsigned char PROGMEM bitmap_bat0[] = {
 B00111111, B11111111, B11111100,
 B10100000, B00000000, B00000100,
 B10100000, B00000000, B00000100,
 B10100000, B00000000, B00000100,
 B00111111, B11111111, B11111100,
};

// 'walk', 24x24px
const unsigned char bitmap_walk [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x0e, 0x00, 0x00, 
	0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0xfc, 0x00, 0x01, 0xfe, 0x00, 0x01, 0xbe, 
	0x00, 0x03, 0x3b, 0x00, 0x03, 0x3b, 0xc0, 0x03, 0x78, 0xe0, 0x00, 0x78, 0x00, 0x00, 0x7c, 0x00, 
	0x00, 0xee, 0x00, 0x00, 0xe6, 0x00, 0x00, 0xc7, 0x00, 0x01, 0x83, 0x00, 0x03, 0x03, 0x80, 0x06, 
	0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'uploadbmp', 32x32px
const unsigned char uploadbmp [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 
	0x00, 0x07, 0xe0, 0x00, 0x00, 0x0d, 0xb0, 0x00, 0x00, 0x09, 0x90, 0x00, 0x00, 0x01, 0x80, 0x00, 
	0x00, 0x01, 0x80, 0x00, 0x07, 0xfd, 0xbf, 0xe0, 0x07, 0xf9, 0x9f, 0xe0, 0x06, 0x01, 0x80, 0x60, 
	0x0e, 0x01, 0x80, 0x70, 0x0e, 0x01, 0x80, 0x70, 0x0e, 0x01, 0x80, 0x70, 0x0e, 0x01, 0x80, 0x70, 
	0x0e, 0x01, 0x80, 0x70, 0x0e, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x70, 
	0x0e, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x70, 0x06, 0x00, 0x00, 0x60, 0x07, 0x00, 0x00, 0xe0, 
	0x07, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void drawBatteryLevel(uint16_t vbat, int16_t v, int16_t y)
{
  display.fillRect(v,y,32,10,SSD1306_BLACK);
  int16_t x = v+5;
  if(vbat>=80){
    display.drawBitmap(x,y,bitmap_bat100,bitmap_bat_width,bitmap_bat_height,SSD1306_WHITE);
  } else if(vbat>=60 && vbat<80) {
    display.drawBitmap(x,y,bitmap_bat75,bitmap_bat_width,bitmap_bat_height,SSD1306_WHITE);
  } else if(vbat>=40 && vbat<60) {
    display.drawBitmap(x,y,bitmap_bat50,bitmap_bat_width,bitmap_bat_height,SSD1306_WHITE);
  } else if(vbat>=20 && vbat<40) {
    display.drawBitmap(x,y,bitmap_bat25,bitmap_bat_width,bitmap_bat_height,SSD1306_WHITE);
  } else if(vbat<20) {
    display.drawBitmap(x,y,bitmap_bat0,bitmap_bat_width,bitmap_bat_height,SSD1306_WHITE);
  }
  //  display.setCursor(v,y+13);
  //  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  //  display.print(vbat);
  //  display.print('%');
   display.display();
}

void clearDisplay(){
  display.clearDisplay();
}

//0, 30
void drawDateTime(uint8_t x, uint8_t y, uint16_t hr, uint16_t min, String ampm, String fulldate){
  display.setFont(&Org_01);
  display.fillRect(x, y-10, 32, 50, SSD1306_BLACK);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(x+25,y+5);
  display.print(ampm[0]);
  display.setCursor(x+25,y+12);
  display.print(ampm[1]);
  display.setTextSize(2);
  display.setCursor(x,y);
  int t = (hr/10)%10;
  // display.print(t);
  t = hr%10;
  display.print(hr);
  display.setCursor(x,y+15);

  t = (min/10)%10;
  display.print(t);
  t = min%10;
  display.print(t);
  
  display.setCursor(x,y+25);
  display.setTextSize(1);
  String d = fulldate.substring(5,11);
  d.replace(" ", ",");
  display.print(d);
  display.display();
}

void drawActivity(int8_t x, int8_t y, uint8_t activity){
  // display.setFont(&Roboto_Bold_10);
  display.fillRect(x,y-5,32,7,SSD1306_BLACK);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(x,y);
  char* a = "";
  switch(activity){
    case 0: break;
    case 1: display.print("IDLE"); break;
    case 2: display.print("WALK"); break;
    case 10: display.print("FALL"); break;
  }
  display.display();
}

void drawWait(uint8_t x, uint8_t y){
  display.fillRect(x,y-5,32,7,SSD1306_BLACK);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(x,y);
  display.print("WAIT..");
  display.display();
}

void drawSteps(uint8_t x, uint8_t y, uint16_t steps){
  display.fillRect(x,y-5,32,6,SSD1306_BLACK);
  // display.drawBitmap(x-8,y,bitmap_walk,24,24,SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(x,y);
  display.setTextColor(SSD1306_WHITE);
  display.print(steps);
  display.setCursor(x,y+7);
  display.print("STEPS");
  display.display();

}

void displaySetup(bool s){
  display.clearDisplay();
  display.drawBitmap(0,s?35:48,bitmap_settings,32,32,SSD1306_WHITE);
  display.setCursor(0, 75);
  s?display.print("SETUP MODE"):display.print("");
  display.display();
}

void displayPower(bool s){
  display.clearDisplay();
  display.drawBitmap(0,s?35:48,bitmap_shutdown,32,32,SSD1306_WHITE);
  display.setCursor(0, 75);
  s?display.print("BYE..."):display.print("");
  display.display();
  // delay(3000);
}

void displayUpload(bool s){
  display.clearDisplay();
  display.drawBitmap(0,s?35:48,uploadbmp,32,32,SSD1306_WHITE);
  display.setCursor(0, 75);
  s?display.print("WAIT..."):display.print("");
  display.display();
  // delay(3000);
}