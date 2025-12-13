#include "UI.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "AudioScope.h"
#include "TextEditor.h"
#include "Dino.h"

#define CARDKB_ADDR 0x5F

extern Adafruit_ST7789 tft;
extern AudioScope audioscope;
extern TextEditor texteditor;
extern Dino dino;

UIElement uiElement;

// Добавляем переменные для управления курсором меню
int currentSelection = 0;
const int menuItemsCount = 6;

void UIElement::mainMenu(){
  //tft.fillScreen(ST77XX_BLACK);
  Wire.requestFrom(CARDKB_ADDR, 1);

  if (Wire.available()) {
    char c = Wire.read();
    
    if (c != 0) {
      if (c == 0xb5) {
        if(currentSelection > 0) {
          currentSelection--;
        }
      }
      else if (c == 0xb6) { 
        if(currentSelection < (sizeof(enumFunc)/sizeof(enumFunc[0])) - 1) {
          currentSelection++;
        }
      } else if (c == 0x0d) {
        switch(currentSelection){
          case 1: 
              texteditor.KeyBoardMain();
              break;
          case 2: 
              dino.mainGame();
              break;
        }
      }
    }
  }
  for(int i = 0; i < (sizeof(enumFunc)/sizeof(enumFunc[0])); i++){
    tft.setCursor(10, i * 20 + 10);
    if(i == currentSelection) {
      tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
      tft.print("> ");
    } else {
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.print("  ");
    }
    
    tft.print(enumFunc[i]);
  }
}



void UIElement::dispInit(){
  tft.init(240, 320);
  tft.invertDisplay(false);    
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  //draw "logo"
  for(int i = 0; i < 320; i++){
    tft.fillCircle(i, (20*cos(i*1024))+170, 2, ST77XX_WHITE);
  }
  tft.setCursor(70, 80);
  tft.setTextSize(4);
  tft.println("HackF*");
  tft.setTextSize(1);
  Wire.begin();
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);

  mainMenu();
}