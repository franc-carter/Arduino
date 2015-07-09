

#include <dht.h>

#define doorPin 11
#define dht_dpin A0 //no ; here. Set equal to channel sensor is on

dht DHT;
int t;
int doorState;
int oldDoorState = 2;

void setup(){
    Serial.begin(9600);
    pinMode(doorPin, INPUT);
    digitalWrite(doorPin, HIGH);
}

void loop(){
  delay(100);
  t = t + 100;
  if (t >= 2000) {  // Device can't be read 'too' quickly
    DHT.read11(dht_dpin);
    Serial.print("H="); Serial.println(DHT.humidity);
    Serial.print("T="); Serial.println(DHT.temperature);
    t = 0;
  }
  doorState = digitalRead(doorPin);
  if (doorState != oldDoorState) {
    Serial.print("D="); Serial.println(doorState);
    oldDoorState = doorState;
  }
}


