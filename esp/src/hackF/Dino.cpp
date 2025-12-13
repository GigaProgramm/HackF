#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "Dino.h"

#define DINOH 18
#define DINOW 20

#define CARDKB_ADDR 0x5F

Dino dino;
extern Adafruit_ST7789 tft;

int16_t dinoPosX = 100-DINOW;
int16_t dinoPosY = 240-DINOH;

int16_t cactusPosX = 320-10;

bool isJump = false;
bool isFall = false;

uint8_t cactusSpeed = 0;

long long scoreTime = 0;

int score = 0;

void Dino::game(){
  if(isJump){
    if(!isFall){
      tft.fillRect(dinoPosX+1, dinoPosY, DINOH, DINOW, 0x0);
      dinoPosY-=5;
      if(dinoPosY == 240-DINOH-30){isFall = true;}
    } else {
      tft.fillRect(dinoPosX+2, dinoPosY-1, DINOH, DINOW, 0x0);
      dinoPosY+=5;
      if(dinoPosY == 240-DINOH){isJump = false; isFall = false;}
    }
  }
  tft.drawRGBBitmap(dinoPosX, dinoPosY, dino, DINOW, DINOH);
  if(cactusPosX <= 0-10){cactusPosX = 320; cactusSpeed = random(6, 10);}
  tft.fillRect(cactusPosX, 240-16, 10, 16, 0x0);
  cactusPosX-=8;
  tft.drawRGBBitmap(cactusPosX, 240-16, cactus, 10, 16);
  score += cactusSpeed;  
}

void Dino::mainGame(){
  tft.fillScreen(ST77XX_BLACK);
  cactusSpeed = random(6, 10);
  while(1){
    Wire.requestFrom(CARDKB_ADDR, 1);
    if (Wire.available()) {
      char c = Wire.read();
      if(c == 0x80){
        tft.fillScreen(ST77XX_BLACK);
        break;
      } else if (c == 0x20) {
        if(!isJump){
          isJump = true;
        }
      }
    }
    if(millis() >= 1000 + scoreTime){
      tft.setCursor(0,0);
      tft.print(score);
    }
    game();
  }
}

