#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

ESP8266WebServer server;
int pin_led = 2;
char* ssid = "";
char* password = "";

void setup () {
  pinMode(pin_led, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.on("/",[](){server.send(200,"text/plain","Hello World!");});
  server.on("/light/status", handleGetLightStatus);
  server.on("/light/toogle", handleToggleLED);
  server.begin();
}
void loop () {
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
  server.send(200, "application/json", getStatus());
}
void handleToggleLED () {
  digitalWrite(pin_led, !digitalRead(pin_led));
  server.send(200, "application/json", getStatus());
}
