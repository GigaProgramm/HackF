#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "TextEditor.h"  

#define CARDKB_ADDR 0x5F

extern Adafruit_ST7789 tft;
TextEditor texteditor;

TextEditor::TextEditor() : 
    inputText(""),
    cursorX(5),
    cursorY(30),
    charWidth(6),
    charHeight(8),
    textAreaTop(30),
    capsLock(false)
{
    
}

void TextEditor::KeyBoardMain() {
  drawUI();
}

void TextEditor::drawUI() {
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 5);
  tft.println("Text editor");
  
  tft.drawLine(0, 20, 320, 20, ST77XX_WHITE);
  
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(5, 305);
  
  cursorX = 5;
  cursorY = 30;
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  checkKeyboard();
}

void TextEditor::checkKeyboard() {
  while(1){
    Wire.requestFrom(CARDKB_ADDR, 1);
    
    if (Wire.available()) {
      char c = Wire.read();
      if(c == 0x80){
        inputText = "";
        break;
      } else if (c != 0) {
        handleKeyPress(c);
      }
    }
    
    delay(50);
  }
}

void TextEditor::handleKeyPress(char key) {
  Serial.print("Key: 0x");
  Serial.println(key, HEX);
  
  switch (key) {
    case 0x8: // Backspace
      backspace();
      break;
      
    case 0x0D: // Enter
      newLine();
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

void TextEditor::addChar(char c) {
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

void TextEditor::backspace() {
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
    tft.print(" ");
    tft.setCursor(cursorX, cursorY);
  }
}

bool TextEditor::isEndOfLine() {
  return cursorX >= 335;
}

void TextEditor::newLine() {
  inputText += '\n';
  cursorX = 5;
  cursorY += charHeight;
  
  if (cursorY + charHeight > 295) {
    scrollUp();
  }
}

void TextEditor::scrollUp() {
  tft.fillRect(0, textAreaTop, 240, 270, ST77XX_BLACK);
  cursorY -= charHeight;
}

void TextEditor::clearAll() {
  inputText = "";
  drawUI();
}

