#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <RF24.h>

RTC_DS3231 rtc;


int Menu = 1;
int cMenu = 0;
int cHour = 99;
int cMinute = 99;
int cSecond = 99;
bool dispUpdate = false;

bool firstStart = false;


#ifdef screenTimeOut
bool DispOn;
int lastActTime;
#endif