#include <Servo.h> 
 
Servo myservo; 

int servoPin = 10;
int PIRPin = 6;
 
void setup() 
{
  myservo.attach(servoPin);
  Serial.begin(9600);
  pinMode(PIRPin, INPUT);
} 
 
void loop() 
{ 
    int motion;
    
    motion = digitalRead(PIRPin);
    Serial.println(motion);
    if (motion) {
      myservo.write(180);
      delay(1000);
    }
    else {
      myservo.write(0);
      delay(1000);
    }
} 

