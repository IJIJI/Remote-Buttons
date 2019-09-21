#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
RTC_DS3231 rtc;

const char modes[10][11] = {"Timed", "Re-end", "Quiz", "Button Set","Time Set", "Pairing", "7", "8", "9", "10"};


#define buttonUp 2
#define buttonDown 3
#define buttonMain 4
#define ledMain 9

int buttons = 1; // default terminals + base stations

//#define menuLoop
#define menuMax 6
#define menuMin 1

#define buttonMax 10

#define screenTimeOut 1

#define adress1 6 // Here the button amount is stored

const int EEPid[6] {99, 240, 129, 92, 7, 12};
const int PairCode[buttonMax] {101, 102, 103, 104, 105, 106, 107, 108, 109, 110};
const int trigCode[buttonMax] {151, 152, 153, 154, 155, 156, 157, 158, 159, 160};

int Menu = 1;
int cMenu = 0;
int cHour = 99;
int cMinute = 99;
int cSecond = 99;
bool dispUpdate = false;

#ifdef screenTimeOut
bool DispOn;
int lastActTime;
#endif

int x;

void setup()
{


  
  //set the pinmodes.
  pinMode(buttonUp, INPUT_PULLUP); 
  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buttonMain, INPUT_PULLUP); 
  pinMode(ledMain, OUTPUT);

  // Print an initialization message to the LCD.
  lcd.init();  // initialize the lcd. 
  lcd.backlight();
  digitalWrite(ledMain, HIGH);
  bool DispOn = true;
  lastActTime = secTime();
  lcd.setCursor(2,0);
  lcd.print("initializing");

  if (EEPROMcheck() == false){
    lcd.print("*");
    
  }

  lcd.setCursor(0,1);
  lcd.print("[              ]"); //Make a status bar
  delay(200);
  for (x; x <= 13; x++){

    lcd.setCursor(x + 1,1);
    lcd.print("=");
    delay(70); 
  }  
  lcd.clear(); //Clear the LCD
  digitalWrite(ledMain, LOW);
}


void loop()
{
  DateTime now = rtc.now(); //define rtc.now
  if (digitalRead(buttonMain) == LOW && DispOn == true){
    prgrm(Menu);
    while(digitalRead(buttonMain) == LOW){delay(1);}
  }
  if (digitalRead(buttonUp) == LOW && DispOn == true){ //check if the up button is pressed
    Menu = Menu + 1;
    while (digitalRead(buttonUp) == LOW){delay(1);}
  }
  if (digitalRead(buttonDown) == LOW && DispOn == true){ //check if the down button is pressed
    Menu = Menu - 1;
    while (digitalRead(buttonDown) == LOW){delay(1);}
  }
  if(Menu > menuMax){
    #ifdef menuLoop
    Menu = menuMin;
    #else
    Menu = menuMax;
    #endif
  }
  if(Menu < menuMin){
    #ifdef menuLoop
    Menu = menuMax;
    #else
    Menu = menuMin;
    #endif
  }
  #ifdef screenTimeOut
  if (digitalRead(buttonDown) == LOW || digitalRead(buttonUp) == LOW || digitalRead(buttonMain) == LOW){
    lastActTime = secTime();
    
    while(digitalRead(buttonDown) == LOW || digitalRead(buttonUp) == LOW || digitalRead(buttonMain) == LOW && DispOn == false){delay(1);}
    
  }
  if (secTime() >= lastActTime + 60 * screenTimeOut && DispOn == true){
    lcd.noBacklight();
    DispOn = false;
  }
  else if (secTime() < lastActTime + 60 * screenTimeOut && DispOn == false){
    lcd.backlight();
    DispOn = true;
    dispUpdate = true;
  }
  #endif


  if (Menu != cMenu || dispUpdate == true){ //check if the supposed menu is different from the current menu
    cMenu = Menu;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Mode: ");
    lcd.print(modes[cMenu - 1]);
    delay(200);
  }
  if (now.hour() != cHour || dispUpdate == true){ //check if the current time is different from the supposed time
    cHour = now.hour();
    lcd.setCursor(4,1);
    lcd.print("   ");
    lcd.setCursor(4,1);
    lcdDigitPrint(now.hour(), 2);
    lcd.print(":");
  }
  if (now.minute() != cMinute || dispUpdate == true){ //check if the current time is different from the supposed time
    cMinute = now.minute();
    lcd.setCursor(7,1);
    lcd.print("   ");
    lcd.setCursor(7,1);
    lcdDigitPrint(now.minute(), 2);
    lcd.print(":");
  }
  if (now.second() != cSecond || dispUpdate == true){ //check if the current time is different from the supposed time
    cSecond = now.second();
    lcd.setCursor(10,1);
    lcd.print("   ");
    lcd.setCursor(10,1);
    lcdDigitPrint(now.second(), 2);

  }

  if (dispUpdate == true){
    dispUpdate = false;
  }
}


void lcdDigitPrint(int inVar, int digits){
  int zeros;
  if (inVar < int(pow(10, digits - 1))){
    for(int x = 1; x < digits; x++){
      if(inVar < int(pow(10, x))){
        zeros = digits - x;
        for(x = 1; x <= zeros; x++){
          lcd.print("0");
        }
        lcd.print(inVar);
        break;
      }

    }

  }
  else{
    lcd.print(inVar);
  }
}

int secTime() {
  DateTime now = rtc.now();
  int secTime;
  secTime = now.second() + now.minute() * 60 + now.hour() * 60 * 60;
  return secTime;
}
int timeDiv(int x){
  DateTime now = rtc.now();
  int result;
  if (x == 0){
    result = now.hour();
  }
  else if (x == 1){
    result = now.minute();
  }
  else if (x == 2){
    result = now.second();
  }
  return result;

}
int EEPROMcheck(){
  bool familiar = true;
  for(int x = 0; x < 6; x++){
    if (EEPROM.read(x) != EEPid[x]){
      familiar = false;
    }
  }
  if (familiar == false){
    for (int x = 0; x < EEPROM.length(); x++){
      EEPROM.write(x, 0);
    }
    for (int x = 0; x < 6; x++){
      EEPROM.write(x, EEPid[x]);
    }
    EEPROM.write(adress1, buttons);
  }
  if (familiar == true){
    buttons = EEPROM.read(adress1);
  }
  return familiar;
}



//VV should be moved to seperate files

void prgrm(int menu) {
  DateTime now = rtc.now();
  
  if (menu == 1)
  {
    int pedButtons = 0;
    int tStart;
    int bMenu = 3;
    int cBMenu = 99;
    int time;
    int actived[buttonMax] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

    bool buttonMainActive = 0;

    while(digitalRead(buttonMain) == LOW){delay(1);}
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time:");
    while(digitalRead(buttonMain) != LOW){
      lcd.setCursor(1,1);
      lcd.print("Press to start");
      
    }
    while(digitalRead(buttonMain) == LOW){delay(1);}
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time:");
    lcd.setCursor(0,1);
    lcd.print("Left:");
    tStart = secTime();
    while(pedButtons < buttons){
      time = secTime() - tStart;
      lcd.setCursor(6, 0);
      lcd.print(time);
      lcd.setCursor(6,1);
      lcd.print(buttons-pedButtons);
      if (digitalRead(buttonMain) == LOW){
        actived[0] = 1;
      }
      for (int x = 1; x <= buttons; x++){
        //check if input car = trigCode[x - 1]
      }
      pedButtons = 0;
      for (int x = 0; x <= buttons; x++){
        pedButtons = pedButtons + actived[x];
      }
      

    }
    lcd.setCursor(6,1);
    lcd.print("Completed!");
    while(digitalRead(buttonMain) == LOW){delay(1);}
    while(digitalRead(buttonMain) != LOW){delay(1);}
    while(digitalRead(buttonMain) == LOW){delay(1);}
    


  }
  else if (menu == 3){
    int bPressd = 999;
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("Quiz");
    while (digitalRead(buttonMain) == LOW){delay(1);}
    while (digitalRead(buttonMain) != LOW){
      lcd.setCursor(1,1);
      lcd.print("Press To Start");
    }

  }
  else if (menu == 4){

    int cButtons = 99;

    while(digitalRead(buttonMain) == LOW){delay(1);}
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Stations:");
    while(digitalRead(buttonMain) != LOW){
      if (digitalRead(buttonUp) == LOW && buttons < buttonMax){
        buttons = buttons + 1;
        while(digitalRead(buttonUp) == LOW){delay(1);}
      }
      if (digitalRead(buttonDown) == LOW && buttons > 1){
        buttons = buttons - 1;
        while(digitalRead(buttonDown) == LOW){delay(1);}
      }

      if (cButtons != buttons){
        cButtons = buttons;
        lcd.setCursor(10, 0);
        lcd.print(buttons);
      }


    }
    EEPROM.write(adress1, buttons);
    dispUpdate = true;
    lastActTime = secTime();

  }
  else if(menu == 5){

    int cTimeEdit = 0;
    char tOffset[3] = {0, 0, 0};
    const char tMaxLimits[3] = {24, 60, 60};
    char cSetTime[3] = {0, 0, 0};
    const char timeLocs[3] = {4, 7, 10};
    while(digitalRead(buttonMain) == LOW){delay(1);}
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time:");
    while(cTimeEdit <= 2){

      if (timeDiv(cTimeEdit) + tOffset[cTimeEdit] != cSetTime[cTimeEdit] || timeDiv(cTimeEdit) + tOffset[cTimeEdit] == 0){ //check if the current time is different from the supposed time
        cSetTime[cTimeEdit] = timeDiv(cTimeEdit) + tOffset[cTimeEdit];
        lcd.setCursor(timeLocs[cTimeEdit],1);
        lcd.print("   ");
        lcd.setCursor(timeLocs[cTimeEdit],1);
        lcdDigitPrint(timeDiv(cTimeEdit) + tOffset[cTimeEdit], 2);
        if (cTimeEdit != 2){ 
          lcd.print(":");
        }
      }
      if (digitalRead(buttonMain) == LOW){
        cTimeEdit = cTimeEdit + 1;
        while(digitalRead(buttonMain) == LOW){delay(1);}
      }
      if (digitalRead(buttonUp) == LOW && timeDiv(cTimeEdit) + tOffset[cTimeEdit] < tMaxLimits[cTimeEdit]){
        tOffset[cTimeEdit] = tOffset[cTimeEdit] + 1;
        while(digitalRead(buttonUp) == LOW){delay(1);}
      }
      if (digitalRead(buttonDown) == LOW && timeDiv(cTimeEdit) + tOffset[cTimeEdit] > 0){ 
        tOffset[cTimeEdit] = tOffset[cTimeEdit] - 1;
        while (digitalRead(buttonDown) == LOW){delay(1);}
      }
      
    }
    int hTime = timeDiv(0) + tOffset[0];
    int mTime = timeDiv(1) + tOffset[1];
    int sTime = timeDiv(2) + tOffset[2];
    rtc.adjust(DateTime(2019, 10, 15, hTime, mTime, sTime));


  }
  else if (menu == 6){
    while (digitalRead(buttonMain) == LOW){delay(1);}
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Hold Pair On");
    lcd.setCursor(0,1);
    lcd.print("Button:");
    for(int x = 2; x <= buttons; x++){
      lcd.setCursor(8,1);
      lcd.print("        ");
      lcd.setCursor(8,1);
      lcd.print(x);
      while (digitalRead(buttonMain) != LOW){}
      // send paircode paircode[x] to transmitters  
      while (digitalRead(buttonMain) == LOW){delay(1);}
    }
    lcd.clear();
    if (buttons > 1){
      lcd.setCursor(0,0);
      lcd.print("Pairing complete!");
    }
    else {
      lcd.setCursor(3,0);
      lcd.print("No Buttons");
      lcd.setCursor(4,1);
      lcd.print("To Pair!");
    }
    while(digitalRead(buttonMain) != LOW){delay(1);}
      
  }
  lcd.clear();
  dispUpdate = true;
  lastActTime = secTime();


}
