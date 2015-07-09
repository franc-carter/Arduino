/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup()
{
  mySwitch.enableTransmit(5);
}

void loop() {
  /* Same switch as above, but using decimal code */
  mySwitch.send(0xDADAD1, 24);
  delay(1000);  
}
