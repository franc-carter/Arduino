
/*
* Simple Receiver Code
* (TX out of Arduino is Digital Pin 1)
* (RX into Arduino is Digital Pin 0)
*/

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  //2400 baud for the 434 model
  Serial.begin(9600);
  mySerial.begin(2400);
}

void loop() {
  // read in values, debug to computer
  if (mySerial.available() > 0) {
    Serial.write(mySerial.read());
  }
  delay(10);
}

