
/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <RCSwitch.h>

#define HOUSE B10101101
#define UNIT  B00000001

RCSwitch mySwitch = RCSwitch();

void spray()
{
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
  delay(400);
  digitalWrite(9,LOW);
  digitalWrite(8,HIGH);
  delay(400);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
}


void setup() {
  Serial.begin(9600);
  pinMode(8,LOW);
  pinMode(9,LOW);
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
}

void loop()
{
  if (mySwitch.available()) {    
    unsigned long value = mySwitch.getReceivedValue();
    int house = (value >> 16);
    int unit  = (value >> 8) & 0xFF;
    if ((house == HOUSE) && (unit == UNIT)) {
      if (value == 0) {
        Serial.print("Unknown encoding");
      } else {
        value = value & 0xFF;
        Serial.println(value);
        spray();
      }
    }
    else {
      Serial.print(value);
      Serial.println(": Not for this system");
    }
    mySwitch.resetAvailable();
  }
}

