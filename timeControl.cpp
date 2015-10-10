#include "timeControl.h"

#include <IRLib.h>
#include <LedControl.h>

#include "Arduino.h"

LedControl lc = LedControl(5, 3, 4, 1);

byte numbers[10][5] = {
  {7, 5, 5, 5, 7},
  {2, 2, 2, 2, 2},
  {7, 1, 7, 4, 7},
  {7, 1, 7, 1, 7},
  {5, 5, 7, 1, 1},
  {7, 4, 7, 1, 7},
  {7, 4, 7, 5, 7},
  {7, 1, 1, 1, 1},
  {7, 5, 7, 5, 7},
  {7, 5, 7, 1, 7}
};

byte *nr, hours, minutes;

void showNumber() {
  byte left, right;
  left = (*nr) / 10;
  right = (*nr) % 10;
  
  int i, number;
  for (i = 0; i < 5; i++) {
    if (left > 10) {
      number = numbers[right][i];
    } else if (right > 10) {
      number = numbers[left][i] << 4;
    }
    else {
      number = (numbers[left][i] << 4) | numbers[right][i];
    }
    lc.setRow(0, i, number);
  }
}


const timeControl::State timeControl::fsmTable[5][4] = {
	/*         evt_down,         evt_empty,        evt_select,            evt_up, */
	{    st_changeHours,       st_setHours,    st_changeHours,    st_changeHours, },/* changeHours */
	{  st_changeMinutes,     st_setMinutes,  st_changeMinutes,  st_changeMinutes, },/* changeMinutes */
	{    st_changeHours,       st_setHours,     st_setMinutes,    st_changeHours, },/* setHours */
	{  st_changeMinutes,     st_setMinutes,      st_setupTime,  st_changeMinutes, },/* setMinutes */
	{      st_setupTime,       st_setHours,      st_setupTime,      st_setupTime, },/* setupTime */
};

void timeControl::pollEvents() {
	/* TODO: update currentEvent */

  if (My_Receiver.GetResults(&My_Decoder)) {
    My_Receiver.resume(); 
    My_Decoder.decode();
    
    switch(My_Decoder.value){
      case 0x61D6D827: currentEvent = evt_up; break;
      case 0x61D6F807: currentEvent = evt_down; break;
      case 0x61D6E817: currentEvent = evt_select; break;
    }
  }
}

void timeControl::action_setMinutes(){
	/* TODO: add action for state setMinutes */
  nr = &minutes;
  if(minutes == 60) minutes = 0;
  if(minutes == 255) minutes = 59;
}
void timeControl::action_changeHours(){
	/* TODO: add action for state changeHours */

  if(currentEvent == evt_up){
    hours++;
  } else if(currentEvent == evt_down){
    hours--;
  }

  currentEvent = evt_empty;
}

void timeControl::action_setHours(){
	/* TODO: add action for state setHours */
  nr = &hours;
  
  if(hours == 24) hours = 0;
  if(hours == 255) hours = 23;
}
void timeControl::action_changeMinutes(){
	/* TODO: add action for state changeMinutes */
  if(currentEvent == evt_up){
    minutes++;
  } else if(currentEvent == evt_down){
    minutes--;
  }

  currentEvent = evt_empty;
}
void timeControl::action_setupTime(){
	/* TODO: add action for state setupTime */
  Serial.print("setup time ");
  Serial.print(hours);
  Serial.print(":");
  Serial.println(minutes);

  currentEvent = evt_empty;
}

void timeControl::evalState(){
	switch(currentState){
		case st_setMinutes:
			action_setMinutes();
			break;
		case st_changeHours:
			action_changeHours();
			break;
		case st_setHours:
			action_setHours();
			break;
		case st_changeMinutes:
			action_changeMinutes();
			break;
		case st_setupTime:
			action_setupTime();
			break;
	}
}

timeControl::timeControl(IRrecv &rec, IRdecode &dec) : My_Receiver(rec), My_Decoder(dec){
	interval = 0;
	previousMillis = 0;
	currentState = st_setupTime;
	currentEvent = evt_none;

  lc.shutdown(0,false);
  lc.setIntensity(0,2);
  lc.clearDisplay(0);
}

void timeControl::loop() {
  lastEvent = currentEvent;
  showNumber();
	pollEvents();
	currentMillis = millis();
	if (evtChanged || currentMillis - previousMillis > interval) {
		previousMillis = currentMillis;
		if(evtChanged && currentEvent != evt_none){ //evt did not change to evt_none
			currentState = (State) fsmTable[currentState][currentEvent];
  	}
    evalState();
	}
 
  evtChanged = currentEvent != lastEvent;
}
