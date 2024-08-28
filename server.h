#include <AsyncTCP.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <fspath.h>
#include <fsreadwrite.h>
#include <ESP32Time.h>
#include "settings_app.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

ESP32Time rtc(0);
bool exitSettings = false;

void runWebServer(){
 WiFi.softAP("activity_tracker", "1234567890");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", "connected");
  });

  server.on("/admin", HTTP_GET, [](AsyncWebServerRequest *request) {
   
    if(request->hasParam("fullname")) writeFile(fullNamePath, request->getParam("fullname")->value().c_str());
    if(request->hasParam("sos1")) writeFile( sos1Path, request->getParam("sos1")->value().c_str());
    if(request->hasParam("net")) writeFile( apnPath, request->getParam("net")->value().c_str());
    
    request->send_P(200, "text/plain", "Saved");
    Serial.println("Settings saved succesfully");
    // writeFile(firstStart, "true");
  });

   server.on("/adminhome", HTTP_GET, [](AsyncWebServerRequest *request) {
   
    request->send_P(200, "text/html", index_html);
    // writeFile(firstStart, "true");
  });

  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request) {
   
    if(request->hasParam("yr")) {
      int hr = request->getParam("h")->value().toInt();
      int min = request->getParam("m")->value().toInt();
      int sec = request->getParam("s")->value().toInt();
      int dt = request->getParam("dat")->value().toInt();
      int mon = request->getParam("mon")->value().toInt();
      int yr = request->getParam("yr")->value().toInt();  
       
      //Serial.printf("Time:%d:%d, Date: %d/%d/%d\n", hr, min, dt, mon+1, yr);
      rtc.setTime(sec, min, hr, dt, mon+1, yr);  // 17th Jan 2021 15:24:30
      delay(1000);

      Serial.print("Time: ");
    Serial.print(rtc.getTime());
    Serial.println();
   
    }
     
    request->send(200, "text/plain", "Saved");
  });

    server.on("/safeadmin", HTTP_GET, [](AsyncWebServerRequest *request) {
   
    if(request->hasParam("sos1")) writeFile( sos1Path, request->getParam("sos1")->value().c_str());
    if(request->hasParam("net")) writeFile( apnPath, request->getParam("net")->value().c_str());
    
    request->send(200, "text/plain", "Saved");
    Serial.println("Settings saved succesfully");
  });

  server.on("/close", HTTP_GET, [](AsyncWebServerRequest *request) {
    //End webserver
    request->send(200, "text/plain", "Done");
    exitSettings = true;
    delay(1000);
  });

  server.begin();

 
}

void endServer(){
// WiFi.softAPdisconnect(true);
ESP.restart();
}

bool settingsAbort(){
 return exitSettings;
}