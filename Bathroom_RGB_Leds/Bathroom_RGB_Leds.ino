
#include <RCSwitch.h>

#define REDPIN 10
#define GREENPIN 9
#define BLUEPIN 11

typedef struct radio_msg_t {
  uint8_t device_id;
  uint8_t cmd;
  uint16_t value;
} radio_msg_t;

RCSwitch    mySwitch = RCSwitch();
int         myID = 7;
byte        R,G,B = 0;
radio_msg_t RadioMsg; // grr - typedef scoping is broken in Arduino

void setup()
{
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  analogWrite(REDPIN, 0);
  analogWrite(GREENPIN, 0);
  analogWrite(BLUEPIN, 0);
  mySwitch.enableReceive(0);
  Serial.begin(9600);
}

int get_radio_msg(void)
{
  if (mySwitch.available()) {
    unsigned long msg = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();
    if (msg != 0) {
      RadioMsg.device_id = (msg & 0xfc0000) >> 18;
      RadioMsg.cmd       = (msg & 0x03e000) >> 13;
      RadioMsg.value     = (msg & 0x001fff);
      return (RadioMsg.device_id == myID);
    }
  }
  return 0;
}

void doCmd(char cmd, byte value)
{
  cmd = cmd + 'A'-1;
  Serial.println(cmd);
  switch(cmd) {
    case 'R': R = value; break;
    case 'G': G = value; break;
    case 'B': B = value; break;
    case 'S':  
      analogWrite(REDPIN, R);
      analogWrite(GREENPIN, G);
      analogWrite(BLUEPIN, B);
      break;
  }
}

void setRGB(int r, int g, int b)
{
  Serial.print("setRGB(");
  doCmd('R'-'A', r); doCmd('G'-'A', g); doCmd('B'-'A', b);
  doCmd('S'-'A', 0);
}

void loop()
{
  if (get_radio_msg())
    doCmd(RadioMsg.cmd,RadioMsg.value);
}

