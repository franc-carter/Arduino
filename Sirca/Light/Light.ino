

int sensorPin = A0;
int light = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
  light = analogRead(sensorPin);
  Serial.println(light);
  delay(500);
}

