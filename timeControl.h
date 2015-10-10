#ifndef timeControl_HEADER
#define timeControl_HEADER

#include <IRLib.h>

class timeControl{
public:
	timeControl(IRrecv &rec, IRdecode &dec);
	void loop();
private:
	typedef enum {evt_down, evt_empty, evt_select, evt_up, evt_none} Event;
	Event currentEvent, lastEvent;

	typedef enum {st_changeHours, st_changeMinutes, st_setHours, st_setMinutes, st_setupTime, } State;
	State currentState;

  IRrecv &My_Receiver;
  IRdecode &My_Decoder;

	static const State fsmTable[5][4];
	unsigned long currentMillis, previousMillis, interval;
	bool evtChanged;
	void pollEvents();
	void action_setMinutes();
	void action_changeHours();
	void action_setHours();
	void action_changeMinutes();
	void action_setupTime();
	void evalState();
};

#endif
