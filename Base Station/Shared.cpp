#include "EEPROM.h"
#include "Config_adv.h"
#include "Pins.h"

void bPress(int sCheck){
  if (sCheck == true){
    while(digitalRead(buttonMain) == LOW){delay(1);}
    while(digitalRead(buttonMain) != LOW){delay(1);}
  }
  digitalWrite(ledMain, HIGH);
  while (digitalRead(buttonMain) == LOW){delay(1);}
  digitalWrite(ledMain, LOW);
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