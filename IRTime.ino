#include <MaxMatrix.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <DCF77.h>
#include <TimeLib.h>

#include "characters.h"

#define DCF_PIN 2           // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0    // Interrupt number associated with pin

time_t time;
DCF77 DCF = DCF77(DCF_PIN, DCF_INTERRUPT);

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

void readTime(byte *minutes, byte *hours) {
  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
  if (DCFtime != 0)
  {
    setTime(DCFtime);
    hours = hour();
    minutes = minute();
  } else {
    minutes = 0;
    hours = 0;
  }
}
void setup() {
  mm.init(); // module initialize
  mm.setIntensity(1); // dot matix intensity 0-15
  mm.clear();

  Wire.begin();
  sensors.begin();

  DCF.Start();
}

void loop() {
  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
  if (DCFtime != 0)
  {
    setTime(DCFtime);
  }
  modeClock();
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

  if (temperature < 0) {
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
  readTime(&minutes, &hours);
  readTemp(&temperature);
  updateDisplay();
  mm.shiftLeft(false, true);
  printStringWithShift(string1, 75);
}

void readTemp(signed char *temp) {
  sensors.requestTemperatures();
  *temp = (signed char) sensors.getTempCByIndex(0);
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
