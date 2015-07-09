


#define MAXBUF 32

char inBuffer[MAXBUF];
int latchPin = 8;
int clockPin = 11;
int dataPin = 12;

void setup()
{
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

byte parseIntList(char *intList)
{
  byte switchBits = 0;
  int sw[5] = {0,0,0,0,0};
  int i;
  
  int n = sscanf(intList, " %d %d %d %d %d", sw, sw+1, sw+2, sw+3, sw+4);
  for (i=0; i<n; i++) {
    if (sw[i] < 1 || sw[i] > 5) {
      Serial.println("Switch numbers must be between 1 and 5");
    }
    else {
      switchBits |= (1<<(sw[i]-1));
    }
  }
  return switchBits;
}

void setSwitch(byte highBytes, byte lowBytes, int d)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, lowBytes);
  shiftOut(dataPin, clockPin, MSBFIRST, highBytes);
  digitalWrite(latchPin, HIGH);
  delay(d);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, (byte)0);
  shiftOut(dataPin, clockPin, MSBFIRST, (byte)0);
  digitalWrite(latchPin, HIGH);
  delay(d);
  digitalWrite(latchPin, LOW);
}

void setOnOff(byte onBits, byte offBits)
{
  int i;

  for(i=0; i<5; i++) {
    byte b = (1 << i);
    if (offBits & b) {
      setSwitch(0,b,1000);
    }
    if (onBits & b) {
      setSwitch(b,0,1000);
    }
  }
}

void loop()
{
  char incomingByte;
  int i = -1;
  byte on, off = 0;

  // setSwitch(B11111111,B11111,1000);
  
  while(i < MAXBUF-1 && inBuffer[i] != '\n') {
    if (Serial.available() > 0) {
      inBuffer[++i] = Serial.read();
    }
  }
  if (inBuffer[i] != '\n') {
    inBuffer[i] = (char)0;
    Serial.println(inBuffer);
    Serial.print("Input too long (");
    Serial.print(i);
    Serial.println(" bytes) junking input");
  }
  else {
    inBuffer[i] = (char)0;
    Serial.println(inBuffer);
    if (!strncmp(inBuffer,"on:",3)) {
      on = parseIntList(inBuffer+3);
    }
    else if (!strncmp(inBuffer,"off:",4)) {
      off = parseIntList(inBuffer+4);
    }
    else {
      Serial.println("Can't parse line");
    }
    if (off > 0) {
      setOnOff(0, off);
    }
    else if (on > 0) {
      setOnOff(on, 0);
    }
  }
}

