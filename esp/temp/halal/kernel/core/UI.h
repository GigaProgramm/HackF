#ifndef UI_H
#define UI_H

#include "Arduino.h"
#include <TFT_eSPI.h>
#include "task.h"
#include "GyverButton.h"

#define BT_LEFT 34
#define BT_RIGHT 35
#define BT_CENTER 32
#define BT_UP 33
#define BT_DOWN 25

#define MenuItem 7
#define setCnt 5

extern TFT_eSPI tft;
extern TaskScheduler scheduler;

class UserInterface {

    static void taskManagerWrapper();
    static void userInterfaceWrapper();
    static void settingsWrapper();

private:
    bool btLeft, btRight, btCenter, btUp, btDown = false;
    int8_t menuPosUI = 0;
    int8_t menuPosTM = 0;
    uint8_t taskCount = 0;
    String currentTask;
public:
    static UserInterface* instance;

    void setStartSetup(){
      pinMode(BT_LEFT, INPUT);
      pinMode(BT_LEFT, INPUT);
      pinMode(BT_LEFT, INPUT);
      pinMode(BT_LEFT, INPUT);
      pinMode(BT_LEFT, INPUT);
    }

    void drawMenu() {
        static String ico[MenuItem] = {"Task Manager", "IR", "BLE", "WiFi", "Lora", "RFID", "Settings"};
        tft.drawRect(15 + (ico[menuPosUI].length() * 6), menuPosUI * 10 + 2, 5, 5, TFT_WHITE);
            if(btUp.isClick() && menuPosUI > 0){
                tft.drawRect(15 + (ico[menuPosUI].length() * 6), menuPosUI * 10 + 2, 5, 5, TFT_BLACK);
                menuPosUI--;
            }
            if(btDown.isClick() && menuPosUI <= MenuItem - 1){
                tft.drawRect(15 + (ico[menuPosUI].length() * 6), menuPosUI * 10 + 2, 5, 5, TFT_BLACK);
                menuPosUI++;
            }
            if(menuPosUI > MenuItem- 1 ){menuPosUI = MenuItem - 1;}
            if(menuPosUI < 0){menuPosUI = 0;}
        

        if(!btUp.isHolded()){
            for(int i = 0; i <= sizeof(ico) - 1; i++){
                tft.setCursor(0, i * 10);
                tft.print(ico[i]);
            }
        
        } else {
            switch(menuPosUI){
                case 0: scheduler.addTask(UserInterface::taskManagerWrapper, "Task Manager");
                        scheduler.endTask(scheduler.findIndexTask("User Interface"));
                        break;
                case 6: scheduler.addTask(UserInterface::settingsWrapper, "Settings");
                        scheduler.endTask(scheduler.findIndexTask("User Interface"));
                        break;
            }
            tft.fillScreen(TFT_BLACK);
        }
    }

    void taskManager(){  // task manager
        btLeft.tick();
        btRight.tick();
        btCenter.tick();
        btUp.tick();
        btDown.tick();
        tft.setCursor(0, 0);
        tft.println("Task manager");
        tft.drawFastHLine(0, 10, 240, TFT_WHITE);
        taskCount = scheduler.getActiveTaskCount();
        for(int i = 0; i < taskCount; i++){
            tft.setCursor(0, i*10 + 12);
            tft.print(scheduler.getTaskName(i));
        }
        tft.drawRect(15 + (scheduler.getTaskName(menuPosTM).length() * 6), menuPosTM * 10 + 15, 5, 5, TFT_WHITE);
        if(btUp.isClick() && menuPosTM > 0){
            tft.drawRect(15 + (scheduler.getTaskName(menuPosTM).length() * 6), menuPosTM * 10 + 15, 5, 5, TFT_BLACK);
            menuPosTM--;
        }
        if(btDown.isClick() && menuPosTM <= taskCount){
            tft.drawRect(15 + (scheduler.getTaskName(menuPosTM).length() * 6), menuPosTM * 10 + 15, 5, 5, TFT_BLACK);
            menuPosTM++;
        }
        if(menuPosTM > taskCount - 1){menuPosTM = taskCount - 1;}
        if(menuPosTM < 0){menuPosTM = 0;}

        if(btDown.isHolded()){
            tft.fillRect(0, 12, 200, 180, TFT_BLACK);
            scheduler.endTask(menuPosTM);
        }
        if(btUp.isHolded()){
            tft.fillScreen(TFT_BLACK);
            scheduler.addTask(UserInterface::userInterfaceWrapper, "User Interface");
            scheduler.endTask(scheduler.findIndexTask("Task Manager"));
        }

    }
    void settings(){
        static String settingPar[setCnt] = {"System color"};
        btLeft.tick();
        btRight.tick();
        btCenter.tick();
        btUp.tick();
        btDown.tick();
        tft.setCursor(0, 0);
        tft.println("Settings");
        tft.drawFastHLine(0, 10, 240, TFT_WHITE);
        if(btUp.isHolded()){
            tft.fillScreen(TFT_BLACK);
            scheduler.addTask(UserInterface::userInterfaceWrapper, "User Interface");
            scheduler.endTask(scheduler.findIndexTask("Settings"));
        }
        for(int i = 0; i < sizeof(settingPar); i++){
            tft.setCursor(0, i*10 + 11);
            tft.print(settingPar[i]);
        }
    }
    void btUpdate(){

    }
};

#endif
UserInterface* UserInterface::instance = nullptr;

void UserInterface::taskManagerWrapper() {
    if (instance) instance->taskManager();
}

void UserInterface::userInterfaceWrapper(){
    if (instance) instance->drawMenu();
}

void UserInterface::settingsWrapper(){
    if (instance) instance->settings();
}

//upd