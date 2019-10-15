#include "Config.h"

#define buttonMax 10

#define menuMax 6
#define menuMin 1

#define setUpAdress 6 // this is how the firmware knows if it has been set up for the first time
#define adress1 7 // Here the button amount is stored

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int EEPid[6] {99, 240, 129, 92, 7, 9};
const int setUpId = 98;

const char modes[menuMax][11] = {"Timed", "Re-end", "Quiz", "Button Set","Time Set", "Pairing"};

int buttons = 1; // default terminals + base stations