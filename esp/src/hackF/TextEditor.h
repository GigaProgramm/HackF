#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <Arduino.h>  

class TextEditor{
  public:
    TextEditor();
    void KeyBoardMain();
    void drawUI();
    void checkKeyboard();
    void handleKeyPress(char key);
    void addChar(char c);
    void backspace();
    bool isEndOfLine();
    void newLine();  
    void scrollUp();
    void clearAll();
    void updateStatus();
  private:
    String inputText;
    int cursorX;
    int cursorY;
    const int charWidth;
    const int charHeight;
    const int textAreaTop;
    bool capsLock;
};

extern TextEditor texteditor;

#endif