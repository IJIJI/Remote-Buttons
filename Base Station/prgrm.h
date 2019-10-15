#include "main.h"
#include "Config_adv.h"
#include "Shared.h"
#include "Pins.h"


void prgrm(int menu) {
  DateTime now = rtc.now();
  
  if (menu == 1 || menu == 2)
  {
    int pedButtons = 0;
    int bMenu = 3;
    int cBMenu = 99;
    float tStart = 0;
    float time = 0;
    int actived[buttonMax] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

    bool buttonMainActive = 0;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time:");
    lcd.setCursor(1,1);
    lcd.print("Press to start");
    while(digitalRead(buttonMain) != LOW){delay(5);}
    digitalWrite(ledMain, HIGH);
    while(digitalRead(buttonMain) == LOW){delay(5);}
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time:");
    lcd.setCursor(0,1);
    lcd.print("Left:");
    tStart = millis();
    while(pedButtons < buttons){
      time = (millis() - tStart) / 1000;
      lcd.setCursor(6, 0);
      lcdDigitPrint(getMin(time), 2);
      lcd.print(":");
      lcdDigitPrint(getSec(time), 2);
      lcd.print(".");
      lcd.print(getMil(time));
      lcd.setCursor(6,1);
      lcd.print(buttons-pedButtons);
      if (digitalRead(buttonMain) == LOW || actived[0] == 1){
        actived[0] = 1;
        digitalWrite(ledMain, LOW);
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
    bPress(true);
    
    


  }
  else if (menu == 3){
    int bPressd = 999;
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("Quiz");
    lcd.setCursor(1,1);
    lcd.print("Press To Start");
    bPress(true);

  }
  else if (menu == 4){

    int cButtons = 99;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Stations:");
    while(digitalRead(buttonMain) == LOW){delay(1);}
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
        lcd.print("       ");
        lcd.setCursor(10, 0);
        lcd.print(buttons);
      }
      

    }
    EEPROM.write(adress1, buttons);
    dispUpdate = true;
    lastActTime = secTime();
    bPress(false);
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
        bPress(false);
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
      bPress(true);
      // send paircode paircode[x] to transmitters  
      // delay(300);
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
    delay(1000);
      
  }
  lcd.clear();
  dispUpdate = true;
  lastActTime = secTime();


}
