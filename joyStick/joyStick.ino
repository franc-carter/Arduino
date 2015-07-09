
int joyPin1 = 0;                 // slider variable connecetd to analog pin 0
int joyPin2 = 1;                 // slider variable connecetd to analog pin 1
int value1 = 0;                  // variable to read the value from the analog pin 0
int value2 = 0;                  // variable to read the value from the analog pin 1
int buttonPin = 2;
int btn = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  Serial.begin(9600);
}

void loop() {
  value1 = analogRead(joyPin1);   
  value2 = analogRead(joyPin2);
  btn = !digitalRead(buttonPin);
  Serial.print("Joy=(");
  Serial.print(value1);
  Serial.print(",");
  Serial.print(value2);
  Serial.println(")");
  Serial.print("button=");
  Serial.println(btn);
  delay(100);
}

