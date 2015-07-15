
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// This contains
//const char* ssid = "mySSID";
//const char* password = "MyWiFiPasswd";
#include "Myauth.h"

const int sprayPin = 3;
const int spray_time = 800;

ESP8266WebServer server(80);

void doSpray()
{
  digitalWrite(sprayPin, HIGH);
  server.send(200, "text/plain", "The world smells beautiful!\n");
  delay(spray_time);
  digitalWrite(sprayPin, LOW);
}

void WifiSetup()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setup(void)
{
  pinMode(sprayPin,OUTPUT);
  digitalWrite(sprayPin, LOW);
  digitalWrite(1,LOW);
  WifiSetup();
  server.on("/spray", doSpray);
  server.begin();
}
 
void loop(void){
  server.handleClient();
} 

