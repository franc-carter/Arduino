

#include <stdint.h>

#define MID 518
#define SENSORPIN  A0
#define SAMPLES    1000
void setup()
{
  analogReference(EXTERNAL);
  pinMode(SENSORPIN,INPUT);
  Serial.begin(9600);
}

float getAmps()
{
  unsigned long total = 0;
  float Arms = 0.0;
  
  for(int i=0; i<SAMPLES; i++) {
      unsigned long n = analogRead(SENSORPIN);
      n = (n < 512) ? (512-n) : (n-512);
      total += n*n;
      delayMicroseconds(800);
  }
  Arms = sqrt((float)(total/SAMPLES))*0.0293-0.03;
  if (Arms < 0)
    Arms = 0.0;

  return Arms;
}

float prev_amps = -1.0;

void loop()
{
  float amps;

  for(int i=0; i<60; i++) {
    amps = getAmps();
    if (abs(amps-prev_amps) > 0.5) {
      Serial.print("A0=");
      Serial.println(amps);
      prev_amps = amps;
    }
  }
  Serial.print("A0=");
  Serial.println(amps);
}

