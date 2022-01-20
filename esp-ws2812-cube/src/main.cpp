
#include <Arduino.h>
#include <Adafruit_Sensor.h>

#include <SPI.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include <FS.h>

#include"cube_mode.h"
#include<mpu6050.h>

//wifi
const char* ssid = "Redmi_1036"; // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "g.13301690897"; // The password of the Wi-Fi network
ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);
//mode
CUBE_MODE cube_mode=CUBE_MODE(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//mpu6050
MPU6050 mpu6050;



//function declare
void handleRoot();
void handleNotFound();
void handleLED();
bool handleFileRead(String path);

void do_without_delay()
{
    if(mpu6050.int_triggered)
    {
        mpu6050.rd_clr_int();
        mpu6050.get_acc_raw();
        cube_mode.cube_get_acc_mode(mpu6050.acc_raw_x,
                                    mpu6050.acc_raw_y,
                                    mpu6050.acc_raw_z);
    }   
    // Serial.print("accx:");
    // Serial.print(mpu6050.acc_raw_x);
    // Serial.print(" accy:");
    // Serial.print(mpu6050.acc_raw_y);
    // Serial.print(" accz:");
    // Serial.println(mpu6050.acc_raw_z);
}

ICACHE_RAM_ATTR void mpu6050_int_cb(){
    mpu6050.int_triggered=true;
}
void setup() {
    // put your setup code here, to run once:
    
    Serial.begin(115200);
    
    cube_mode.begin();

    if(!mpu6050.begin())
        Serial.println("mpu6050 init failed");
    mpu6050.cycle_mode(true,MPU6050_CYCLE_20_HZ);
    mpu6050.enable_int(true);
    attachInterrupt(digitalPinToInterrupt(D5), mpu6050_int_cb, RISING);

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

}

void loop() {
    // put your main code here, to run repeatedly:
    cube_mode.clear();
    cube_mode.mode_apply();
    do_without_delay();
    // server.handleClient();

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

