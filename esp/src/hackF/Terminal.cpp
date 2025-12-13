#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "Terminal.h"  
#include "AudioScope.h"
#include "TextEditor.h"
#include "Dino.h"

#define CARDKB_ADDR 0x5F

extern Adafruit_ST7789 tft;
extern AudioScope audioscope;
extern TextEditor texteditor;
extern Dino dino;

Terminal terminal;

void Terminal::run(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.print("terminal");
  tft.drawLine(0,10, 320, 10, ST77XX_WHITE);
  checkKeyboard();
}

void Terminal::checkKeyboard() {
  while(1){
    Wire.requestFrom(CARDKB_ADDR, 1);
    
    if (Wire.available()) {
      char c = Wire.read();
      if(c == 0x80){
        tft.fillScreen(ST77XX_BLACK);
        break;
      } else if (c != 0) {
        handleKeyPress(c);
      }
    }
    
    delay(50);
  }
}

void Terminal::handleKeyPress(char key) {
  Serial.print("Key: 0x");
  Serial.println(key, HEX);
  Serial.println(inputText);
  
  switch (key) {
    case 0x8: // Backspace
      backspace();
      break;
      
    case 0xA3: // New line
      newLine();
      break;

    case 0x0D: // Enter command
      cmdParse();
      break;

    case 0x1B: // ESC
      clearAll();
      break;
      
    default:
      if (key >= 32 && key <= 126) {
        addChar(key);
      }
      break;
  }
}

void Terminal::addChar(char c) {

  if (capsLock && c >= 'a' && c <= 'z') {
    c = toupper(c);
  }
  
  inputText += c;

  if (cursorX + charWidth > 235) {
    newLine();
  }
  
  if (cursorY + charHeight > 295) {
    scrollUp();
  }
  
  tft.setCursor(cursorX, cursorY);
  tft.print(c);
  cursorX += charWidth;
}

void Terminal::backspace() {
  if (inputText.length() > 0) {
    inputText.remove(inputText.length() - 1);
    
    if (cursorX > 5) {
      cursorX -= charWidth;
    } else if (cursorY > textAreaTop) {
      cursorY -= charHeight;
      cursorX = 335;

      while (cursorX > 5 && !isEndOfLine()) {
        cursorX -= charWidth;
      }
    }

    tft.setCursor(cursorX, cursorY);
    tft.println(" ");
    tft.setCursor(cursorX, cursorY);
  }
}

bool Terminal::isEndOfLine() {
  return cursorX >= 335;
}

void Terminal::newLine() {
  inputText += '\n';
  cursorX = 5;
  cursorY += charHeight;
  
  if (cursorY + charHeight > 295) {
    scrollUp();
  }
}

void Terminal::scrollUp() {
  tft.fillRect(0, textAreaTop, 240, 270, ST77XX_BLACK);
  cursorY -= charHeight;
}

void Terminal::clearAll() {
  inputText = "";
  run();
}
void