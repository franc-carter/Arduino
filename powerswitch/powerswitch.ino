#include <PinChangeInt.h>

// These two pins are connected for interrupts.
// Add more Pins at your leisure.
// For the Analog Input pins, you can use 14, 15, 16, etc.
// or you can use A0, A1, A2, etc.  The Arduino code comes with #define's
// for the Analog Input pins.
#define PIN1 1
#define PIN2 2

uint8_t latest_interrupted_pin;
uint8_t interrupt_count[20]={0}; // 20 possible arduino pins

void quicfunc() {
  latest_interrupted_pin=PCintPort::arduinoPin;
  interrupt_count[latest_interrupted_pin]++;
};

void setup() {
  // pinMode(PIN1, INPUT); digitalWrite(PIN1, HIGH);
  // PCintPort::attachInterrupt(PIN1, &quicfunc, FALLING);  // add more attachInterrupt code as required
  pinMode(2, INPUT); //digitalWrite(PIN2, HIGH);
  PCintPort::attachInterrupt(2, &quicfunc, CHANGE);
  Serial.begin(9600);
  Serial.println("---------------------------------------");
}

uint8_t i;

void loop() {
  uint8_t count;
  delay(1000);
  //Serial.println("Start:");
  for (i=0; i < 20; i++) {
    //Serial.print("i = ");
    //Serial.println(i);
    if (interrupt_count[i] != 0) {
      count=interrupt_count[i];
      interrupt_count[i]=0;
      Serial.print("Count for pin ");
      if (i < 14) {
        Serial.print("D");
        Serial.print(i, DEC);
      } else {
        Serial.print("A");
        Serial.print(i-14, DEC);
      }
      Serial.print(" is ");
      Serial.println(count, DEC);
    }
  }
}

