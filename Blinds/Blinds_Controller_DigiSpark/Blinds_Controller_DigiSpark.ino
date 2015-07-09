/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <stdint.h>
#include <DigiUSB.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

uint8_t dstID;
uint8_t cmd;
uint16_t value;

void setup()
{
  DigiUSB.begin();
  mySwitch.enableTransmit(5);
}

#define BUFSIZE 32
int get_cmd()
{
  int n = 0;
  int pos = -1;
  char buf[BUFSIZE];
  
  int byte = -1;
  while (n < BUFSIZE) {
    if (DigiUSB.available()) {
      int ch = DigiUSB.read();
      if (ch > 0) {
        buf[n] = (char)ch;
        if ((char)ch == '\n')
          break;
        n++;
      }
      DigiUSB.delay(10);
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















void get_input() {
  int lastRead;
  // when there are no characters to read, or the character isn't a newline
  while (true) { // loop forever
    if (DigiUSB.available()) {
      // something to read
      lastRead = DigiUSB.read();
      DigiUSB.write(lastRead);
      
      if (lastRead == '\n') {
        break; // when we get a newline, break out of loop
      }
    }
    
    // refresh the usb port for 10 milliseconds
    DigiUSB.delay(10);
  }
}



