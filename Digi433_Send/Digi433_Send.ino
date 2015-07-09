
// Send a message using RCSwitch
// Message is 24 bits long
//
//   DestID(6) Cmd(5)    Value(13)
//  ----------------------------------
//  | XXXXXX | XXXXX | XXXXXXXXXXXXX |
//  ----------------------------------
//
//  Read from DigiUSB: (DestID),(Cmd),(Value)
//
//    (\d+).(.).(\d+)
//


#include <stdlib.h>
#include <RCSwitch.h>
#include <DigiUSB.h>

#define MAX_CMD_LEN 20

RCSwitch mySwitch = RCSwitch();

char buf[MAX_CMD_LEN+1];

void setup()
{
  DigiUSB.begin();
  mySwitch.enableTransmit(5);  
}

void writeStr(char *str)
{
  while (*str != (char)0)
    DigiUSB.write(*str);
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

char toUpper(char ch)
{
  if (ch >= 'a' && ch <= 'z')
    ch = ch - ('a'-'A');
  return ch;
}

int find_int()
{
  int n = 0;
  
  DigiUSB.write('I');
  while ((buf[n] != (char)0) && isDigit(buf[n])) {
    DigiUSB.write('I');
    n++;
  }
  buf[n] = (char)0;
  
  return n;
}

int readline()
{
  int n = 0;
  
  while (n <= MAX_CMD_LEN) {
    DigiUSB.write('.');
    if (DigiUSB.available()){
      buf[n] = (char)DigiUSB.read();
      if (buf[n] == '\n') {
        buf[n] = (char)0;
        return n;
      }
      n++;
      DigiUSB.refresh();
      delay(10);
    }
  }
  buf[MAX_CMD_LEN+1] = (char)0;

  return -1;
}

void loop()
{
  uint32_t msg, dstID, cmd, value;
  
  int len = readline();
  if (len >= 5) {
    DigiUSB.write('L');
    writeStr(buf);
    int pos = find_int();
    if (pos > 1) {
      DigiUSB.write('P');
      dstID = (uint32_t)atol(buf) & 0x3F;
      if (pos < len-3) {
        cmd = ((uint32_t)(toUpper(buf[pos+1])-'A') & 0x1F) ;
        value = atol(buf+pos+3) & 0x1FFF;
        msg   = value | (cmd << 13) | (dstID << 18);
        DigiUSB.write(msg);
        doSend(msg, 1, 0);
      }
    }
  }
}

