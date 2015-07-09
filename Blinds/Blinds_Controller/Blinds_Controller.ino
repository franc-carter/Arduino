/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <stdint.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

uint8_t dstID;
uint8_t cmd;
uint16_t value;

void setup()
{
  Serial.begin(9600);
  mySwitch.enableTransmit(4);
}

#define BUFSIZE 32
int get_cmd()
{
  int n = 0;
  int pos = -1;
  char buf[BUFSIZE];
  
  int byte = -1;
  while (n < BUFSIZE) {
    int ch = Serial.read();
    if (ch > 0) {
      buf[n] = (char)ch;
      if ((char)ch == '\n')
         break;
      n++;
    }
  }
  buf[n] = (char)0;
  for(n=1; isDigit(buf[n]); n++);
  buf[n] = (char)0;
  
  cmd   = (uint32_t)(buf[0]-'A'+1);
  dstID = (uint32_t)atoi(buf+1);
  value = (uint32_t)atoi(buf+n+1);
  
  return 1;
}

void loop()
{
  int pos = -1;
  uint32_t msg;
    
  if (get_cmd()) {
    msg   = ((uint32_t)dstID << 20) | ((uint32_t)cmd << 16) | (uint32_t)value; 
    mySwitch.send(msg, 24);
  }
}

