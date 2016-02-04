#include <IRLib.h>
#include <MaxMatrix.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
#include "rtc.h"
#include "characters.h"
#include "codes.h"

int RECV_PIN = 9;

IRrecv My_Receiver(RECV_PIN);
IRdecode My_Decoder;

#define M_CLOCK 1
#define M_SETUP 2
#define S_MINUTES 4
#define S_HOURS 8

int mode, valueSetup;

int data = 12;    // 8, DIN pin of MAX7219 module
int load = 11;    // 9, CS pin of MAX7219 module
int clock = 10;  // 10, CLK pin of MAX7219 module

int maxInUse = 2;    //change this variable to set how many MAX7219's you'll use

byte hours, minutes;

MaxMatrix mm(data, load, clock, maxInUse); // define module

byte buffer[10];

// active sentenses
char string1[] = "00:00 -99C    ";

OneWire oneWire(3);
signed char temperature;
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

void setup() {
  My_Receiver.enableIRIn();

  mm.init(); // module initialize
  mm.setIntensity(1); // dot matix intensity 0-15
  mm.clear();

  mode = M_SETUP;
  valueSetup = S_HOURS;

  Wire.begin();
  sensors.begin();
}

void loop() {
  if (mode == M_CLOCK) {
    modeClock();
  } else {
    modeSetup();
  }
}

bool buttonPressed() {
  if (My_Receiver.GetResults(&My_Decoder)) {
    My_Receiver.resume();
    My_Decoder.decode();

    switch (My_Decoder.value) {
      case VOL_UP: {
          if (valueSetup == S_HOURS) {
            hours++;
          } else if (valueSetup == S_MINUTES) {
            minutes++;
          }
        } break;//UP
      case VOL_DOWN: {
          if (valueSetup == S_HOURS) {
            hours--;
          } else if (valueSetup == S_MINUTES) {
            minutes--;
          }
        } break;//DOWN
      case RETURN: return true; //SETUP
    }
  }
  return false;
}

void updateDisplay() {
  byte h1 = hours / 10;
  byte h2 = hours % 10;

  byte m1 = minutes / 10;
  byte m2 = minutes % 10;

  string1[0] = h1 + '0';
  string1[1] = h2 + '0';
  string1[3] = m1 + '0';
  string1[4] = m2 + '0';

  if(temperature < 0){
    string1[6] = '-';
    temperature *= -1;
  } else {
    string1[6] = ' ';
  }

  byte t1 = temperature / 10;
  byte t2 = temperature % 10;

  string1[7] = t1 + '0';
  string1[8] = t2 + '0';
}

void modeClock() {
  delay(4000);
  readDS3231time(&minutes, &hours);
  readTemp(&temperature);
  updateDisplay();
  mm.shiftLeft(false, true);
  printStringWithShift(string1, 75);
}

void readTemp(signed char *temp){
  sensors.requestTemperatures();
  *temp = (signed char) sensors.getTempCByIndex(0);
}

void modeSetup() {
  if (buttonPressed()) {
    if (valueSetup == S_HOURS) {
      valueSetup = S_MINUTES;
    } else if (valueSetup == S_MINUTES) {
      mode = M_CLOCK;
      valueSetup = S_HOURS;
      setDS3231time(minutes, hours);
    }
  } 

  if (minutes == 60) {
    minutes = 0;
  }

  if (minutes == 255) {
    minutes = 59;
  }

  if (hours == 24) {
    hours = 0;
  }

  if (hours == 255) {
    hours = 23;
  }

  updateDisplay();

  if (valueSetup == S_HOURS) {
    printChar(string1[0], 0);
    printChar(string1[1], 4);
    mm.setDot(0, 7, 1);
    mm.setDot(7, 7, 1);
  } else if (valueSetup == S_MINUTES) {
    printChar(string1[3], 0);
    printChar(string1[4], 4);
  }
}

void printChar(char c, byte x) {
  if (c < 32) return;
  c -= 32;
  memcpy_P(buffer, CH + 7 * c, 7);
  mm.writeSprite(x, 0, buffer);
}

void printCharWithShift(char c, int shift_speed) {
  if (c < 32) return;
  c -= 32;
  memcpy_P(buffer, CH + 7 * c, 7);
  mm.writeSprite(maxInUse * 8, 0, buffer);
  mm.setColumn(maxInUse * 8 + buffer[0], 0);

  for (int i = 0; i < buffer[0] + 1; i++)
  {
    delay(shift_speed);
    mm.shiftLeft(false, false);
  }
}

void printStringWithShift(char* s, int shift_speed) {
  while (*s != 0) {
    printCharWithShift(*s, shift_speed);
    s++;
  }
}
