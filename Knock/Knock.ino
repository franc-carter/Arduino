
unsigned long now = 0;
unsigned long report_interval = 1000;
unsigned long prev_report_t = 0;

const int vibrationSensor = A0; // the piezo is connected to analog pin 0

void setup() {
 Serial.begin(9600);       // use the serial port
}

static unsigned long vibrationSum = 0;
void loop() {
  int sensorReading = analogRead(vibrationSensor);
  vibrationSum = vibrationSum + sensorReading;
  now = millis();
  if (now-prev_report_t > report_interval) {
    Serial.println(vibrationSum);
    vibrationSum = 0;
    prev_report_t = now;
  }
  
}

