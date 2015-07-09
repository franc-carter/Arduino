
#include <Servo.h>

Servo myservo1;
Servo myservo2;

int servo1 = 8;
int servo2 = 9;

void setup()
{
   Serial.begin(9600);
   myservo1.attach(servo1,450,2400);
   myservo2.attach(servo2,450,2400);
}

void loop()
{
  int a;
  
  for(a=0; a<=179; a++) {
    Serial.println(a);
    myservo1.write(a);
    myservo2.write(a);
    delay(15);
  }
  for(a=179; a>=0; a--) {
    Serial.println(a);
    myservo1.write(a);
    myservo2.write(a);
    delay(15);
  }
}


