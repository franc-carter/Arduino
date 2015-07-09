
//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;

volatile int updated = 0;
volatile int N = 0;

volatile int pin1Hi = 1;
volatile int pin2Hi = 1;

void rising1()
{
  static unsigned long interrupt_call_time = 0;
  unsigned long this_call_time = millis();
  if (this_call_time - interrupt_call_time > 50) {
      interrupt_call_time = this_call_time;
      updated = 1;
      pin1Hi = 1;
  }
}

void falling1()
{
  static unsigned long interrupt_call_time = 0;
  unsigned long this_call_time = millis();
  if (this_call_time - interrupt_call_time > 50) {
      interrupt_call_time = this_call_time;
      pin1Hi = 0;
      if (!pin2Hi) {
          updated = 1;
          N--;
      }
  }
}

void rising2()
{
  static unsigned long interrupt_call_time = 0;
  unsigned long this_call_time = millis();
  if (this_call_time - interrupt_call_time > 50) {
      interrupt_call_time = this_call_time;
      pin2Hi = 1;
  }
}

void falling2()
{
  static unsigned long interrupt_call_time = 0;
  unsigned long this_call_time = millis();
  if (this_call_time - interrupt_call_time > 50) {
      interrupt_call_time = this_call_time;
      pin2Hi = 0;
      if (!pin1Hi) {
          N++;
          updated = 1; 
      }
  }
}


void setup() {
  Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

 // digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
 //digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, rising1, RISING); 
  attachInterrupt(0, falling1, FALLING); 
  attachInterrupt(1, rising2, RISING);
  attachInterrupt(1, falling2, FALLING);
}

void loop(){ 
  if (updated) {
    Serial.print("N=");
    Serial.println(N);
    updated = 0;
  }
  delay(100);
}





