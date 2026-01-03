#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "Terminal.h"  
#include "AudioScope.h"
#include "TextEditor.h"
#include "Dino.h"
#include "HackfWiFi.h"

#define CARDKB_ADDR 0x5F

extern Adafruit_ST7789 tft;
extern AudioScope audioscope;
extern TextEditor texteditor;
extern Dino dino;
extern HackfWiFi hackfwifi;

Terminal terminal;

void(* resetFunc) (void) = 0;

void Terminal::run(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(0,0);
  tft.print("terminal");
  tft.drawLine(0,10, 320, 10, ST77XX_WHITE);
  cursorX = 5;
  cursorY = 15;
  checkKeyboard();
}

void Terminal::checkKeyboard() {
  tft.setCursor(cursorX, cursorY);
  tft.print(">");
  cursorX += charWidth;
  while(1){
    Wire.requestFrom(CARDKB_ADDR, 1);
    
    if (Wire.available()) {
      char c = Wire.read();
      if (c != 0) {
        handleKeyPress(c);
      }
    }
    
    delay(50);
  }
}

void Terminal::handleKeyPress(char key) {
  Serial.print("Key: 0x");
  Serial.println(key, HEX);
  
  switch (key) {
    case 0x8: // Backspace
      backspace();
      break;
      
    case 0xA3: // New line
      newLine();
      break;

    case 0x0D: // Enter command
      cmdParse(inputText);
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
  Serial.println(inputText);
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
void Terminal::cmdParse(String input) {
  String cmd = input;
  cmd.trim();
  Serial.print('@');
  Serial.println(cmd);

  Serial.print(':');
  Serial.println(cmd);

  if(cmd == "texteditor") {
      texteditor.KeyBoardMain();
      inputText = "";
      Terminal::run();
  }
  else if(cmd == "dino"){
    dino.mainGame();
    inputText = "";
    Terminal::run();
  }
  else if(cmd == "hreboot"){
    resetFunc();// потом запихну ребут в отдельный класс 
  }
  else if(cmd == "wscan"){
    wifiScanParse();
  }
  else {
      newLine();
      tft.setCursor(cursorX, cursorY);
      tft.setTextColor(ST77XX_RED);
      tft.print("no command in scope");
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      newLine();
      inputText = "";
      checkKeyboard();
  }
}

void Terminal::wifiScanParse(){
    tft.setTextColor(ST77XX_YELLOW);
    String input = hackfwifi.scan();
    int numLine = 1;
    if(input == ""){
      newLine();
      tft.print("no networks found");
      newLine();
    } else {
      numLine = (input.substring(0, input.indexOf(','))).toInt();
      input = input.substring(input.indexOf(',')+1, input.length());
      tft.print('\n' + input);
      for(int i = 0; i <= numLine; i++){
        newLine();
      }
    }
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    inputText = "";
    checkKeyboard();
}


