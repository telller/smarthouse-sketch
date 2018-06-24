#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>

ESP8266WebServer server;
int white = 0;
int yellow = 2;
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

  pinMode(white, OUTPUT);
  pinMode(yellow, OUTPUT);
  server.on("/",[](){server.send(200,"text/plain","Hello World!");});
  server.on("/light/status", getStatus);
  server.on("/light/toogle", handleToggleLED);
  server.begin();
}
void loop () {
  ArduinoOTA.handle();
  server.handleClient();
}
void getStatus () {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& light = jsonBuffer.createObject();
  light["white"] = analogRead(white);
  light["yellow"] = analogRead(yellow);
  String retJson;
  light.printTo(retJson);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "*");
  server.sendHeader("Access-Control-Allow-Headers", "*");
  server.send(200, "application/json", retJson);
}
void handleToggleLED () {
  String data = server.arg("plain");
  DynamicJsonBuffer jBuffer;
  JsonObject& jObject = jBuffer.parseObject(data);
  int whitePwm = jObject["white"];
  int yellowPwm = jObject["yellow"];
  analogWrite(white, whitePwm);
  analogWrite(yellow, yellowPwm);
  getStatus();
}
