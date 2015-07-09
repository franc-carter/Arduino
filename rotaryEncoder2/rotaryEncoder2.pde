
//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;

void setup() {
  Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
}

void loop(){ 
  //Do stuff here

  int MSB = digitalRead(encoderPin1);
  int LSB = digitalRead(encoderPin2);
  Serial.print("MSB=");
  Serial.println(MSB);
  Serial.print("LSB=");
  Serial.println(LSB);
  Serial.println();
  delay(1000); //just here to slow down the output, and show it will work  even during a delay
}

