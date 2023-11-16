#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>


#include <LiquidCrystal.h>
#include <EEPROM.h>

//defining seconds count
#define SEC (60 +(millis() / 1000))

//define keypad buttons
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


class Timer 
{

 public:
  Timer(int rs, int en, int d4, int d5, int d6, int d7, int buttonWhite, int buttonBlack, int analogPin);
  
    void debounceKey();
  //keypad code by:
  //https://www.dfrobot.com/wiki/index.php/Arduino_LCD_KeyPad_Shield_(SKU:_DFR0009)
  //read keypad buttons
    int read_lcd_buttons();
  
  //set up menu
    void menuSetUp();
  
    //printing white values to LCD and print timer to set up menu
    void printTimerWhite();
    
    //printing black values to LCD
    void printTimerBlack();
    void printMenu();
    //printing labels
    void printLabels();
    void pauseGameWhite();
    
    void pauseGameBlack();
    void timerWhite();
    
    //running timer black
    void timerBlack();
    //increment to white
    void incWhite();
    //increment to black
    void incBlack();
    void readEeprom();
    
    void writeEeprom();
  
    void initTimer();
  
  //Arduino loop
    void loopTimer();

    
  private:
    //constants
    int _buttonWhite;
    int _buttonBlack;
    // const int pinBuzzer = 10;
    int _analogPin;
    int _rs, _en, _d4, _d5, _d6, _d7;
    LiquidCrystal lcd;
    
    //Variables for keypad and menu
    int lcd_key = 0;
    int adc_key_in = 0;
    int countMenu = 0;
    int blinkTime;
    int initBlinkTime;
    int blinkDelay = 500;
    bool exitMenu = true;
    bool sidePlayer = true;
    bool blinkState = true;
    
    //Variables for seconds count
    int cTemp = 0;
    int cSecWhite = 0;
    int cSecBlack = 0;
    
    //Increment variable
    int setInc;
    
    //Variables to white timer
    int setSecWhite;
    int horWhite;
    int minWhite;
    int secWhite;
    
    //Variables to black timer
    int setSecBlack;
    int horBlack;
    int minBlack;
    int secBlack;
};
#endif
