

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

byte switches[] = {1,2,4,8,16};

void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void setSwitch(byte lowBits, byte highBits)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, lowBits);
  shiftOut(dataPin, clockPin, MSBFIRST, highBits);
  digitalWrite(latchPin, HIGH);
  Serial.println("sleep");
  delay(1000);
  digitalWrite(latchPin, LOW);
}

void setSwitches(byte turnOn, byte turnOff)
{
  int i;

  Serial.print("On: ");
  Serial.println(turnOn);
  Serial.print("Off: ");
  Serial.println(turnOff);
  
  for(i=0; i<5; i++) {
    if (switches[i] & turnOff) {
      Serial.print("Turn Off: ");
      Serial.println(i+1);
      setSwitch(0, switches[i]);
    }
    else {
      if (switches[i] & turnOn) {
        Serial.print("Turn On: ");
        Serial.println(i+1);
        setSwitch(switches[i], 0);
      }
    }
  }
}

void loop()
{
  setSwitches(1,0);
  delay(1000);
  setSwitches(0,1);
  delay(1000);
  //setSwitch(B10000000,B10000000);
}

