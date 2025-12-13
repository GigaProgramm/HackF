#ifndef UI_H
#define UI_H
#include "Arduino.h"

class UIElement{
public:
  void mainMenu();
  void dispInit();
private:

  String enumFunc[3] = {"AudioScope", "Text editor", "Dino"};
  int currentSelection;
};

extern UIElement uiElement; 

#endif