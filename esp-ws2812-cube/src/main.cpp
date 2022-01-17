#include <Arduino.h>


#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include <FS.h>

#include"cube_mode.h"
 
CUBE_MODE cube_mode=CUBE_MODE(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_I2CDevice i2c_dev = Adafruit_I2CDevice(0x68);
int led = 14;
//wifi
const char* ssid = "Redmi_1036"; // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "g.13301690897"; // The password of the Wi-Fi network
ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);
void handleRoot();
void handleNotFound();
void handleLED();
bool handleFileRead(String path);
void setup() {
    // put your setup code here, to run once:
    // pinMode(led,OUTPUT);
    // pinMode(PIN,OUTPUT);
    //  pinMode(4,OUTPUT);
    //  pinMode(5,OUTPUT);
    //  pinMode(12,OUTPUT);
    //  pinMode(13,OUTPUT);
    //  pinMode(14,OUTPUT);
    //  pinMode(15,OUTPUT);
    // strip.Begin();
    // strip.Show();
    
    Serial.begin(115200);
   // cube_mode.begin();
    cube_mode.begin();
    Serial.println('\n');
    //WiFi.begin(ssid, password);
   // wifiMulti.addAP(ssid, password);
    //WiFi.softAP(ssid,password);
    // Serial.print("Access Point \"");
    // Serial.print(ssid);
    // Serial.println("\" started");
    // Serial.print("IP address:\t");
    // Serial.println(WiFi.softAPIP());
    // while(wifiMulti.run() != WL_CONNECTED)
    // {
    //     delay(1000);
    // }
    // Serial.print("IP address:\t");
    // Serial.println(WiFi.localIP());
    // if (!MDNS.begin("esp8266")) { // Start the mDNS responder for esp8266.local !!!这个功能我的没有实现，不知道什么原因
    //     Serial.println("Error setting up MDNS responder!");
    // }
    // Serial.println("mDNS responder started");   
    // SPIFFS.begin();
    // // server.on("/",HTTP_GET,handleRoot);
    // // server.on("/LED",HTTP_POST,handleLED);
    // server.onNotFound([](){
    //     if (!handleFileRead(server.uri()))
    //         server.send(404, "text/plain", "404: Not Found"); 
    // });
    // server.begin();
    // Serial.println("HTTP server started");
    // Serial.print("Connecting to ");
    // Serial.print(ssid); Serial.println(" ...");

    Serial.println("I2C address detection test");

    if (!i2c_dev.begin()) {
        Serial.print("Did not find device at 0x");
        Serial.println(i2c_dev.address(), HEX);
        while (1);
    }
    Serial.print("Device found on address 0x");
    Serial.println(i2c_dev.address(), HEX);
}

void loop() {
    cube_mode.clear();
   // server.handleClient();
    //cube_mode.cube_breathe_led_all(10);
    cube_mode.cube_do_all_mode();
    // digitalWrite(2,!digitalRead(2));
    //  digitalWrite(4,!digitalRead(4));
    //   digitalWrite(5,!digitalRead(5));
    //    digitalWrite(12,!digitalRead(12));
    //     digitalWrite(13,!digitalRead(13));
    //  digitalWrite(14,!digitalRead(14));
    //   digitalWrite(15,!digitalRead(15));
    //   delay(500);
  // put your main code here, to run repeatedly:
  // digitalWrite(ledpin,HIGH);
  // delay(1000);
  // digitalWrite(ledpin,LOW);
  // delay(1000);
  
   Serial.println("hhh\n");
//   for(int i=0;i<8;i++)
//   {
//        cube_mode.setPixelColor(i,cube_mode.Color(0, 150, 0));
//   }
//   cube_mode.show();
//   delay(1000);
   
//    Serial.println("red\n");
//   for(int i=0;i<8;i++)
//   {
//       cube_mode.setPixelColor(i,COLOR(150, 0, 0));
//   }
//   cube_mode.show();
//   delay(1000);  
//   Serial.println("blue\n");
//   for(int i=0;i<8;i++)
//   {
//        cube_mode.setPixelColor(i,COLOR(0, 0, 150));
//   }
//   cube_mode.show();
//   delay(1000);

}

String getContentType(String filename) { // convert the file extension to the MIME type
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    return "text/plain";
}


void handleRoot() {
    server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\"value=\"Toggle LED\"></form>");
}
void handleLED(){
    digitalWrite(led,!digitalRead(led));
    server.sendHeader("Location","/");
    server.send(303);
}
void handleNotFound(){
    server.send(404, "text/plain", "404: Not found");
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
    Serial.println("handleFileRead: " + path);
    if (path.endsWith("/")) path += "index.html"; // If a folder is requested, send the index file
    String contentType = getContentType(path); // Get the MIME type
    if (SPIFFS.exists(path)) { // If the file exists
        File file = SPIFFS.open(path, "r"); // Open it
        size_t sent = server.streamFile(file, contentType); // And send it to the client
        file.close(); // Then close the file again
        return true;
    }
    Serial.println("\tFile Not Found");
    return false; // If the file doesn't exist, return false
}
void do_without_delay()
{}
