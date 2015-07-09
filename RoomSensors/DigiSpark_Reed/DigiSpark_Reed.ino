
#include <dht.h>
#include <DigiUSB.h>

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
    DigiUSB.begin();
    for(int i=0; i<nPins; i++) {
      pinMode(doorPin[i], INPUT);
      digitalWrite(doorPin[i], HIGH);
    }
}

void loop(){
  t = t + check_interval;
  if (t >= report_interval) {  // Device can't be read 'too' quickly
    DHT.read11(dht_dpin);
    DigiUSB.print("H="); DigiUSB.println(DHT.humidity);
    DigiUSB.print("T="); DigiUSB.println(DHT.temperature);
    for(int i=0; i<nPins; i++) {
        DigiUSB.print("D"); DigiUSB.print(doorPin[i]); DigiUSB.print("="); DigiUSB.println(doorState[i]);
    }
    t = 0;
  }
  for(int i=0; i<nPins; i++) {
    doorState[i] = digitalRead(doorPin[i]);
    if (doorState[i] != oldDoorState[i]) {
      DigiUSB.print("D"); DigiUSB.print(doorPin[i]); DigiUSB.print("="); DigiUSB.println(doorState[i]);
      oldDoorState[i] = doorState[i];
    }
  }
  delay(check_interval);
}

