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
int16_t cactusPosY = 240-16;

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

  tft.fillRect(cactusPosX, cactusPosY, 10, 16, 0x0);
  cactusPosX-=8;

  tft.drawRGBBitmap(cactusPosX, cactusPosY, cactus, 10, 16);
  score += cactusSpeed;  

  if((dinoPosX - cactusPosX > -15 && dinoPosX - cactusPosX < 20) && (cactusPosY- dinoPosY < 15)){
    Serial.println(":::::");
    Serial.println(dinoPosX - cactusPosX);
    Serial.println(dinoPosX - cactusPosX);
    Serial.println(dinoPosY - cactusPosY);
    Serial.println(":::::");
    endGame();
  }
}

void Dino::mainGame(){
  tft.fillScreen(ST77XX_BLACK);
  cactusSpeed = random(6, 10);
  while(1){
    Wire.requestFrom(CARDKB_ADDR, 1);
    if (Wire.available()) {
      char c = Wire.read();
      if(c == 0x80){
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

void Dino::endGame(){
  Serial.println("game over");
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  tft.setTextSize(3);
  tft.setCursor(80, 80);
  tft.print("GAME OVER!");
  while(1){
    Wire.requestFrom(CARDKB_ADDR, 1);
    if (Wire.available()) {
      char c = Wire.read();
      if (c == 0x20) {
        break;
      }
    }
    delay(10);
  }
  cactusPosX = 320;
  score = 0;
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.fillScreen(ST77XX_BLACK);
}



