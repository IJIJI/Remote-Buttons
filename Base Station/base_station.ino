
#include "Shared.h"
#include "Pins.h"
#include "Config_adv.h"
#include "prgrm.h"
#include "Main.h"

/*
System with display on uses about 56ma.

System without display uses about 41ma.
*/

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
    firstStart = true;
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
  if (firstStart == true || EEPROM.read(setUpAdress) != setUpId){
    prgrm(5);
    prgrm(4);
    if (buttons > 1){
      prgrm(6);
    }
    EEPROM.write(setUpAdress, setUpId);
  }
}


void loop()
{
  DateTime now = rtc.now(); //define rtc.now
  if (digitalRead(buttonMain) == LOW && DispOn == true){
    bPress(false);
    prgrm(Menu);
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


int getMil(float sec){
  int sec2 = sec * 100;
  sec = sec2 % 100;
  return abs(sec);
}
int getSec(float sec){
  int sec2 = sec;
  sec = sec2 % 60;
  return sec;

}
int getMin(float sec){
  int sec2 = sec;
  int mins;
  mins = round((sec2 / 60));
  return mins;
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




//VV should be moved to seperate files
