#include <IRLib.h>
#include <MaxMatrix.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

#include "characters.h"

int RECV_PIN = 11;

IRrecv My_Receiver(RECV_PIN);
IRdecode My_Decoder;

#define BTN_SETUP 1
#define BTN_UP 2
#define BTN_DOWN 3

#define M_CLOCK 1
#define M_SETUP 2
#define S_MINUTES 4
#define S_HOURS 8

int mode, valueSetup;


int data = 10;    // 8, DIN pin of MAX7219 module
int load = 9;    // 9, CS pin of MAX7219 module
int clock = 8;  // 10, CLK pin of MAX7219 module

int maxInUse = 2;    //change this variable to set how many MAX7219's you'll use

int hours, minutes, masterClock;

MaxMatrix mm(data, load, clock, maxInUse); // define module

byte buffer[10];

// active sentenses
char string1[] = "00:00      ";

volatile int s = 0;
volatile unsigned int m = 0;
volatile unsigned int h = 0;
volatile boolean interrupt = false;


void setup() {
  My_Receiver.enableIRIn();

  mm.init(); // module initialize
  mm.setIntensity(1); // dot matix intensity 0-15
  mm.clear();

  mode = M_SETUP;
  valueSetup = S_HOURS;

  attachInterrupt(0, clockCounter, RISING);
  analogReference(DEFAULT);

  analogWrite(5, 127);
}

void loop() {
  if (mode == M_CLOCK) {
    modeClock();
  } else {
    modeSetup();
  }
}

bool buttonPressed(byte pin) {
  if (My_Receiver.GetResults(&My_Decoder)) {
    My_Receiver.resume();
    My_Decoder.decode();

    switch (My_Decoder.value) {
      case 0x61D6D827: {
          if (valueSetup == S_HOURS) {
            h++;
          } else if (valueSetup == S_MINUTES) {
            m++;
          }
        } break;//UP
      case 0x61D6F807: {
          if (valueSetup == S_HOURS) {
            h--;
          } else if (valueSetup == S_MINUTES) {
            m--;
          }
        } break;//DOWN
      case 0x61D6E817: return true; //SETUP
    }
  }
  return false;
}

void updateClock() {

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    hours = h;
    minutes = m;
  }

  int h1 = hours / 10;
  int h2 = hours % 10;

  int m1 = minutes / 10;
  int m2 = minutes % 10;

  string1[0] = h1 + '0';
  string1[1] = h2 + '0';
  string1[3] = m1 + '0';
  string1[4] = m2 + '0';

}

void modeClock() {
  delay(4000);
  updateClock();
  mm.shiftLeft(false, true);
  printStringWithShift(string1, 75);
}

void modeSetup() {
  if (buttonPressed(BTN_SETUP)) {
    if (valueSetup == S_HOURS) {
      valueSetup = S_MINUTES;
    } else if (valueSetup == S_MINUTES) {
      mode = M_CLOCK;
      valueSetup = S_HOURS;
    }
  } 

  if (m == 60) {
    m = 0;
  }

  if (m == 255) {
    m = 59;
  }

  if (h == 24) {
    h = 0;
  }

  if (h == 255) {
    h = 23;
  }

  updateClock();

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


void clockCounter()      // called by interrupt
{
  masterClock ++;        // with each clock rise add 1 to masterclock count
  if (masterClock == 977) // 490Hz reached
  {
    s++;          // after one 490Hz cycle add 1 second ;)
    masterClock = 0;     // Reset after 1 second is reached

    if (s == 60) {
      s = 0;
      m++;
    }

    if (m == 60) {
      m = 0;
      h++;
      s++;
    }

    if (h == 24) {
      h = 0;
    }
  }
}
