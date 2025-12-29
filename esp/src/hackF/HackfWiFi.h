#ifndef HACKF_WIFI_H
#define HACKF_WIFI_H

#include <Arduino.h>

class HackfWiFi {
  public:
    String scan();
    int numWeb();
  private:
    int scanResult;
};

extern HackfWiFi hackfwifi;

#endif