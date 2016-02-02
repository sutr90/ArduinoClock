#include <MaxMatrix.h>
#include <IRLib.h>
#include "timeControl.h"

int RECV_PIN = 11;

IRrecv My_Receiver(RECV_PIN);
IRdecode My_Decoder;

timeControl tc(My_Receiver, My_Decoder);

void setup()
{
  Serial.begin(9600);
  My_Receiver.enableIRIn(); // Start the receiver
}

void loop() {
tc.loop();
}

