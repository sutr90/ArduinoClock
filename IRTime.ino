#include <IRLib.h>

#include <MaxMatrix.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

const PROGMEM unsigned char CH[] = {
  4, 8, B00000000, B00000000, B00000000, B00000000, B00000000, // space
  1, 8, B01011111, B00000000, B00000000, B00000000, B00000000, // !
  3, 8, B00000011, B00000000, B00000011, B00000000, B00000000, // "
  5, 8, B00010100, B00111110, B00010100, B00111110, B00010100, // #
  4, 8, B00100100, B01101010, B00101011, B00010010, B00000000, // $
  5, 8, B01100011, B00010011, B00001000, B01100100, B01100011, // %
  5, 8, B00110110, B01001001, B01010110, B00100000, B01010000, // &
  1, 8, B00000011, B00000000, B00000000, B00000000, B00000000, // '
  3, 8, B00011100, B00100010, B01000001, B00000000, B00000000, // (
  3, 8, B01000001, B00100010, B00011100, B00000000, B00000000, // )
  5, 8, B00101000, B00011000, B00001110, B00011000, B00101000, // *
  5, 8, B00001000, B00001000, B00111110, B00001000, B00001000, // +
  2, 8, B10110000, B01110000, B00000000, B00000000, B00000000, // ,
  4, 8, B00001000, B00001000, B00001000, B00001000, B00000000, // -
  2, 8, B01100000, B01100000, B00000000, B00000000, B00000000, // .
  4, 8, B01100000, B00011000, B00000110, B00000001, B00000000, // /
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // 0
  4, 8, B01000010, B01111111, B01000000, B00000000, B00000000, // 1
  4, 8, B01100010, B01010001, B01001001, B01000110, B00000000, // 2
  4, 8, B00100010, B01000001, B01001001, B00110110, B00000000, // 3
  4, 8, B00011000, B00010100, B00010010, B01111111, B00000000, // 4
  4, 8, B00100111, B01000101, B01000101, B00111001, B00000000, // 5
  4, 8, B00111110, B01001001, B01001001, B00110000, B00000000, // 6
  4, 8, B01100001, B00010001, B00001001, B00000111, B00000000, // 7
  4, 8, B00110110, B01001001, B01001001, B00110110, B00000000, // 8
  4, 8, B00000110, B01001001, B01001001, B00111110, B00000000, // 9
  //2, 8, B01010000, B00000000, B00000000, B00000000, B00000000, // :
  2, 8, B01101100, B01101100, B00000000, B00000000, B00000000, // :
  2, 8, B10000000, B01010000, B00000000, B00000000, B00000000, // ;
  3, 8, B00010000, B00101000, B01000100, B00000000, B00000000, // <
  3, 8, B00010100, B00010100, B00010100, B00000000, B00000000, // =
  3, 8, B01000100, B00101000, B00010000, B00000000, B00000000, // >
  4, 8, B00000010, B01011001, B00001001, B00000110, B00000000, // ?
  5, 8, B00111110, B01001001, B01010101, B01011101, B00001110, // @
  4, 8, B01111110, B00010001, B00010001, B01111110, B00000000, // A
  4, 8, B01111111, B01001001, B01001001, B00110110, B00000000, // B
  4, 8, B00111110, B01000001, B01000001, B00100010, B00000000, // C
  4, 8, B01111111, B01000001, B01000001, B00111110, B00000000, // D
  4, 8, B01111111, B01001001, B01001001, B01000001, B00000000, // E
  4, 8, B01111111, B00001001, B00001001, B00000001, B00000000, // F
  4, 8, B00111110, B01000001, B01001001, B01111010, B00000000, // G
  4, 8, B01111111, B00001000, B00001000, B01111111, B00000000, // H
  3, 8, B01000001, B01111111, B01000001, B00000000, B00000000, // I
  4, 8, B00110000, B01000000, B01000001, B00111111, B00000000, // J
  4, 8, B01111111, B00001000, B00010100, B01100011, B00000000, // K
  4, 8, B01111111, B01000000, B01000000, B01000000, B00000000, // L
  5, 8, B01111111, B00000010, B00001100, B00000010, B01111111, // M
  5, 8, B01111111, B00000100, B00001000, B00010000, B01111111, // N
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // O
  4, 8, B01111111, B00001001, B00001001, B00000110, B00000000, // P
  4, 8, B00111110, B01000001, B01000001, B10111110, B00000000, // Q
  4, 8, B01111111, B00001001, B00001001, B01110110, B00000000, // R
  4, 8, B01000110, B01001001, B01001001, B00110010, B00000000, // S
  5, 8, B00000001, B00000001, B01111111, B00000001, B00000001, // T
  4, 8, B00111111, B01000000, B01000000, B00111111, B00000000, // U
  5, 8, B00001111, B00110000, B01000000, B00110000, B00001111, // V
  5, 8, B00111111, B01000000, B00111000, B01000000, B00111111, // W
  5, 8, B01100011, B00010100, B00001000, B00010100, B01100011, // X
  5, 8, B00000111, B00001000, B01110000, B00001000, B00000111, // Y
  4, 8, B01100001, B01010001, B01001001, B01000111, B00000000, // Z
  2, 8, B01111111, B01000001, B00000000, B00000000, B00000000, // [
  4, 8, B00000001, B00000110, B00011000, B01100000, B00000000, // \ backslash
  2, 8, B01000001, B01111111, B00000000, B00000000, B00000000, // ]
  3, 8, B00000010, B00000001, B00000010, B00000000, B00000000, // hat
  4, 8, B01000000, B01000000, B01000000, B01000000, B00000000, // _
  2, 8, B00000001, B00000010, B00000000, B00000000, B00000000, // `
  4, 8, B00100000, B01010100, B01010100, B01111000, B00000000, // a
  4, 8, B01111111, B01000100, B01000100, B00111000, B00000000, // b
  4, 8, B00111000, B01000100, B01000100, B00101000, B00000000, // c
  4, 8, B00111000, B01000100, B01000100, B01111111, B00000000, // d
  4, 8, B00111000, B01010100, B01010100, B00011000, B00000000, // e
  3, 8, B00000100, B01111110, B00000101, B00000000, B00000000, // f
  4, 8, B10011000, B10100100, B10100100, B01111000, B00000000, // g
  4, 8, B01111111, B00000100, B00000100, B01111000, B00000000, // h
  3, 8, B01000100, B01111101, B01000000, B00000000, B00000000, // i
  4, 8, B01000000, B10000000, B10000100, B01111101, B00000000, // j
  4, 8, B01111111, B00010000, B00101000, B01000100, B00000000, // k
  3, 8, B01000001, B01111111, B01000000, B00000000, B00000000, // l
  5, 8, B01111100, B00000100, B01111100, B00000100, B01111000, // m
  4, 8, B01111100, B00000100, B00000100, B01111000, B00000000, // n
  4, 8, B00111000, B01000100, B01000100, B00111000, B00000000, // o
  4, 8, B11111100, B00100100, B00100100, B00011000, B00000000, // p
  4, 8, B00011000, B00100100, B00100100, B11111100, B00000000, // q
  4, 8, B01111100, B00001000, B00000100, B00000100, B00000000, // r
  4, 8, B01001000, B01010100, B01010100, B00100100, B00000000, // s
  3, 8, B00000100, B00111111, B01000100, B00000000, B00000000, // t
  4, 8, B00111100, B01000000, B01000000, B01111100, B00000000, // u
  5, 8, B00011100, B00100000, B01000000, B00100000, B00011100, // v
  5, 8, B00111100, B01000000, B00111100, B01000000, B00111100, // w
  5, 8, B01000100, B00101000, B00010000, B00101000, B01000100, // x
  4, 8, B10011100, B10100000, B10100000, B01111100, B00000000, // y
  3, 8, B01100100, B01010100, B01001100, B00000000, B00000000, // z
  3, 8, B00001000, B00110110, B01000001, B00000000, B00000000, // {
  1, 8, B01111111, B00000000, B00000000, B00000000, B00000000, // |
  3, 8, B01000001, B00110110, B00001000, B00000000, B00000000, // }
  4, 8, B00001000, B00000100, B00001000, B00000100, B00000000, // ~
};

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