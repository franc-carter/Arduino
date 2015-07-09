
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);

  mySwitch.enableTransmit(10);
}

String int2str(int n)
{ 
  String myStr;
  int zeros = 8 - String(n,BIN).length();

  for (int i=0; i<zeros; i++)
    myStr += "0";
  myStr += String(n,BIN);

  return myStr;
}

void loop()
{
  if (Serial.available()) {
    int House = Serial.parseInt();
    int Unit  = Serial.parseInt();
    int n     = Serial.parseInt();
    if (House > 0 && Unit > 0 && n > 0) {
      String msg = int2str(House) + int2str(Unit) + int2str(n);
      int str_len = msg.length() + 1; 
      char char_msg[str_len]; 
      msg.toCharArray(char_msg, 25);
Serial.println(char_msg);
      mySwitch.send(char_msg);
    }
  }
}

