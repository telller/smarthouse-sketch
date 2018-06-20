#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>

ESP8266WebServer server;
int pin_led = 2;
char* ssid = "";
char* password = "";

void setup () {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setHostname("ESP8266-00001");
  ArduinoOTA.begin();

  pinMode(pin_led, OUTPUT);
  server.on("/",[](){server.send(200,"text/plain","Hello World!");});
  server.on("/light/status", handleGetLightStatus);
  server.on("/light/toogle", handleToggleLED);
  server.begin();
}
void loop () {
  ArduinoOTA.handle();
  server.handleClient();
}
String getStatus () {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& light = jsonBuffer.createObject();
  light["state"] = digitalRead(pin_led);
  String retJson;
  light.printTo(retJson);
  return retJson;
}
void handleGetLightStatus () {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "*");
  server.sendHeader("Access-Control-Allow-Headers", "*");
  server.send(200, "application/json", getStatus());
}
void handleToggleLED () {
  digitalWrite(pin_led, !digitalRead(pin_led));
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "*");
  server.sendHeader("Access-Control-Allow-Headers", "*");
  server.send(200, "application/json", getStatus());
}
