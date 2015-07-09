
#include <math.h>

float StepTime  = 0.01; // A 'step' is this number of seconds long
float CycleTime = 35.0; // The whole cycle repeats every this seconds

// Define how a Tardis behaves
typedef struct Tardis {
  int pin;          // LED is connected to this pin
  float startAt;    // Start pulsing at this many seconds
  float endAt;      // Stop pulsing at this meany seconds
  int cycles;       // Do this many cycles of pulse
  int defaultValue; // In case we want to change the default state
} Tardis_Type;

Tardis_Type Tardis[] = {
  {11, 10.0, 20.0, 10, 0},
  {10, 15.0, 25.0, 10, 0},
  {9,  20.0, 30.0, 10, 0},
};

/*
(exp(sin(x))-1/e)*(255/(e-1/e))
*/

int nTardis = sizeof(Tardis)/sizeof(Tardis_Type);

void setup()
{
  for(int n=0; n<nTardis; n++) {
    pinMode(Tardis[n].pin, OUTPUT);
  }
  Serial.begin(9600);
}

int stepN = 0;
void loop()
{
  float now = stepN * StepTime;
  
  for(int n=0; n<nTardis; n++) {
    if ((now >= Tardis[n].startAt) && (now <= Tardis[n].endAt)) { 
      float x = (now-Tardis[n].startAt)/(Tardis[n].endAt-Tardis[n].startAt)*Tardis[n].cycles;
      float dimBy = 1.0 * (Tardis[n].cycles - int(x))/Tardis[n].cycles;
      x = 3*PI/2 + 2*PI*x;
      int val = int(sin(x)*127.0+127.0);
      val = int(float(val) * dimBy*dimBy);
      if (n == 0) {
        Serial.println(val);
      }
      analogWrite(Tardis[n].pin, val);
    }
    else {
      analogWrite(Tardis[n].pin, Tardis[n].defaultValue);
    }
  }
  delay(StepTime*1000);
  stepN = (stepN+1) % int(CycleTime/StepTime);
}

