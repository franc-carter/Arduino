

/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <RCSwitch.h>

#define HOUSE B10101101
#define UNIT  B00000001
#define GOPIN 8

RCSwitch mySwitch = RCSwitch();

void spray()
{
  digitalWrite(GOPIN,HIGH);
  delay(800);
  digitalWrite(GOPIN,LOW);
}


void setup() {
  Serial.begin(9600);
  pinMode(GOPIN,LOW);
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

