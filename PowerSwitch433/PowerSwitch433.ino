/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
*/

#include <stdlib.h>
#include <Digi_RCSwitch.h>
#include <DigiUSB.h>

RCSwitch mySwitch = RCSwitch();

void setup()
{
  //Serial.begin(9600);
  DigiUSB.begin();
  mySwitch.enableTransmit(5);  
}

void doSend(long code, int n, int d)
{
  while(1) {
     mySwitch.send(code, 24);
     if (--n <= 0)
       break;
     delay(d);
  }
}

void get_input()
{
  int n = 0;
  char buf[20];
  long code;
  while (1==1) {
    if(DigiUSB.available()){
      buf[n] = DigiUSB.read();
      if(buf[n] == '\n') {
        buf[n] = '\0';
        code = atol((char*)buf);
        n = 0;
        doSend(code, 4, 250);
        break;
      }
      else
          n++;
    }
    DigiUSB.refresh();
    delay(10);
  }
}

void loop()
{
  get_input();
  //dosend(288092);
  //dosend(288084);
}


