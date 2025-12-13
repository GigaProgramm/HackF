#include "Terminal.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_DC D5
#define TFT_CS D6
#define TFT_MOSI D8
#define TFT_RST D4
#define TFT_MISO D3
#define TFT_SCLK D7


Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
extern Terminal terminal;

void setup() {
  Serial.begin(115200);
  dispInit();
}

void loop() {
  terminal.run();
}

void dispInit(){
  tft.init(240, 320);
  tft.invertDisplay(false);    
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  //draw "logo"
  tft.setCursor(100,10);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("...start system...");
  tft.setCursor(0,0);
  Wire.begin();
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
}