#ifndef TERMINAL_H
#define TERMINAL_H

#include <Arduino.h>  

class Terminal{
  public:
    void run();
    void terminalMain(); 
    void checkKeyboard();
    void handleKeyPress(char key);
    void addChar(char c);
    void backspace();
    bool isEndOfLine();
    void newLine();  
    void scrollUp();
    void clearAll();
    void cmdParse(String input);
  private:
    String inputText = "";
    int cursorX = 5;
    int cursorY = 15;
    const int charWidth = 6;
    const int charHeight = 8;
    const int textAreaTop = 30;
    bool capsLock;
};

extern Terminal terminal;

#endif