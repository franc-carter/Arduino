
#include <DigiUSB.h>

int inPin = 5;
int val = 0;

void setup()
{
  pinMode(inPin, INPUT);      // sets the digital pin 7 as input
  Serial.begin(9600);
}

void loop()
{
  val = digitalRead(inPin);   // read the input pin
  if (val)
    DigiUSB.write('1');
  else
    DigiUSB.write('0');
  DigiUSB.write('\n');

  delay(1000);
}

