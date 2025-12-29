#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HackfWiFi.h"

HackfWiFi hackfwifi;

String HackfWiFi::scan() {

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  String result = "\n";

  int n = WiFi.scanNetworks();

  if (n == 0){
    Serial.println("no networks found");
    result = "no networks found";
  } else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; i++)
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      result += WiFi.SSID(i) + ((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*") + "\n";
      delay(10);
    }
  }
  Serial.println("");
  return result;
}

int HackfWiFi::numWeb(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  int n = WiFi.scanNetworks();
  return n;
}