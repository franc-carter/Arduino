
/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <RCSwitch.h>

#define HOUSE  173
#define UNIT   1
#define GOPIN  8
#define LEDPIN 13

RCSwitch mySwitch = RCSwitch();

void spray(int n)
{
  while(n-- > 0) {
    digitalWrite(LEDPIN,HIGH);
    digitalWrite(GOPIN,HIGH);
    delay(800);
    digitalWrite(GOPIN,LOW);
    digitalWrite(LEDPIN,LOW);
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(GOPIN,OUTPUT);
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
}

void loop()
{
  if (mySwitch.available()) {    
    unsigned long value = mySwitch.getReceivedValue();
Serial.println(value);
    int house = (value >> 16);
    int unit  = (value >> 8) & 0xFF;
Serial.println(house);
Serial.println(unit);
    if ((house == HOUSE) && (unit == UNIT)) {
      if (value == 0) {
        Serial.print("Unknown encoding");
      } else {
        int num_sprays = value & 0xFF;
Serial.println(num_sprays);
        spray(num_sprays);
      }
    }
    else {
      Serial.print(value);
      Serial.println(": Not for this system");
    }
    mySwitch.resetAvailable();
  }
}

