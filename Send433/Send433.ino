
// Send a message using RCSwitch
// Message is 24 bits long
//
//   DestID(6) Cmd(5)    Value(13)
//  ----------------------------------
//  | XXXXXX | XXXXX | XXXXXXXXXXXXX |
//  ----------------------------------
//
//  Read from Serial: (DestID),(Cmd),(Value)
//
//    (\d+).(.).(\d+)
//


#include <stdlib.h>
#include <RCSwitch.h>

#define MAX_CMD_LEN 20
#define XMIT_PIN 10

RCSwitch mySwitch = RCSwitch();

char buf[MAX_CMD_LEN+1];

void setup()
{
  mySwitch.enableTransmit(XMIT_PIN);
  Serial.begin(9600);  
}

void send_cmd(uint32_t dstID, uint32_t cmd, uint32_t value, int n, int d)
{
  uint32_t msg = (value & 0x1FFF) | ((cmd & 0x1F) << 13) | ( (dstID & 0x3F) << 18);
  while(1) {
     mySwitch.send(msg, 24);
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
  
  while ((buf[n] != (char)0) && isDigit(buf[n]))
    n++;
  buf[n] = (char)0;

  return n;
}

void setRGB(int r, int g, int b)
{
  doSend(7, 'R', r, 1, 0); delay(100);
  doSend(7, 'G', g, 1, 0); delay(100);
  doSend(7, 'B', b, 1, 0); delay(100);
  doSend(7, 'S', 0, 1, 0);
}

void process_cmd()
{
  uint32_t msg, dstID, cmd, value;

  int len = Serial.readBytesUntil('\n',buf,MAX_CMD_LEN);
  buf [len] = (char)0;
  if (len >= 5) {
    int pos = find_int();
    if (pos >= 1) {
      dstID = (uint32_t)atol(buf) & 0x3F;
      if (pos < len-3) {
        cmd   = toUpper(buf[pos+1]) ;
        value = atol(buf+pos+3) & 0x1FFF;
        send_cmd(dstID,cmd,value,1,0);
      }
    }
  }
}

void test_it()
{
  doSend(7,'B', 255, 1, 0); delay(100);
  doSend(7,'S', 0, 1, 0);
  delay(1000);
  doSend(7, 'B', 0, 1, 0); delay(100);
  doSend(7,'S', 0, 1, 0);
  delay(1000);
}

void loop()
{

  // test_it()
  process_cmd();

}



