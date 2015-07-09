
#include <dht.h>

#define dht_dpin A0 //no ; here. Set equal to channel sensor is on

unsigned int t;

int doorPin[]     = {12,11};
int doorState[]    = { 0, 0};
int oldDoorState[] = { -1, -1};

#define nPins (sizeof(doorPin)/sizeof(int))


dht DHT;
int check_interval = 100;  // (in milliseconds)
int report_interval = 1 * 60 * 1000; // report every 1 minutes

void setup(){
    Serial.begin(9600);
    for(int i=0; i<nPins; i++) {
      pinMode(doorPin[i], INPUT);
      digitalWrite(doorPin[i], HIGH);
    }
}

void loop(){
  t = t + check_interval;
  if (t >= report_interval) {  // Device can't be read 'too' quickly
    DHT.read11(dht_dpin);
    Serial.print("H="); Serial.println(DHT.humidity);
    Serial.print("T="); Serial.println(DHT.temperature);
    for(int i=0; i<nPins; i++) {
        Serial.print("D"); Serial.print(doorPin[i]); Serial.print("="); Serial.println(doorState[i]);
    }
    t = 0;
  }
  for(int i=0; i<nPins; i++) {
    doorState[i] = digitalRead(doorPin[i]);
    if (doorState[i] != oldDoorState[i]) {
      Serial.print("D"); Serial.print(doorPin[i]); Serial.print("="); Serial.println(doorState[i]);
      oldDoorState[i] = doorState[i];
    }
  }
  delay(check_interval);
}

