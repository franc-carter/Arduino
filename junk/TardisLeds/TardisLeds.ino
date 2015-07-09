
#include <math.h>

float StepTime  = 0.01;
float CycleTime = 5.0;

// const float PI = 3.141592;

typedef struct Tardis {
  int pin;
  float startAt;
  float endAt;
  int cycles;
  int defaultValue;
} Tardis_Type;

Tardis_Type Tardis[] = {
  {11, 1.0, 3.0, 1, 0},
  {10, 2.0, 4.0, 1, 0},
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
      float x = PI * (now-Tardis[n].startAt)/(Tardis[n].endAt-Tardis[n].startAt);
      int val = int(sin(x)*255);
      //int val = int((exp(sin(x)) - 0.36787944)*108.0);
      //int val = sin(x)
      analogWrite(Tardis[n].pin, val);
    }
    else {
      analogWrite(Tardis[n].pin, Tardis[n].defaultValue);
    }
  }
  delay(StepTime*1000);
  stepN = (stepN+1) % int(CycleTime/StepTime);
}

