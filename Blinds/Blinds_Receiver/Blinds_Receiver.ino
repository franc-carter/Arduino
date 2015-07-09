/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
}

void loop() {
  //Serial.println("Loop");
  if (mySwitch.available()) {
    unsigned long msg = mySwitch.getReceivedValue();
    if (msg == 0) {
      Serial.print("Unknown encoding");
    } else {
      unsigned long dstID = (msg & 0xf00000) >> 20; 
      unsigned long cmd   = (msg & 0x0f0000) >> 16;
      unsigned long val   = (msg & 0x00ffff);
      
      Serial.print("Received: "); Serial.println(msg);
      Serial.print("    dstID: "); Serial.print(dstID);
      Serial.print("    cmd: "); Serial.print(cmd);
      Serial.print("    val: "); Serial.println(val);
      Serial.println();
    }
    mySwitch.resetAvailable();
  }
}
