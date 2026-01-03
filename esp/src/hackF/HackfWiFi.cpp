#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HackfWiFi.h"

HackfWiFi hackfwifi;

String HackfWiFi::scan() {

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  String result = "";

  int n = WiFi.scanNetworks();

  if (n == 0){
    Serial.println("no networks found");
    return "";
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
      result += WiFi.SSID(i) + "(" + WiFi.RSSI(i) + ")" + ((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*") + "\n";
      delay(10);
    }
    result = String(n) + "," + result;
  }
  Serial.println("");
  return result;
}