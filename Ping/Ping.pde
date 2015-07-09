
#include <OneWire.h>

int tempPin = 8;
int pingPin = 7;

void setup()
{
  Serial.begin(9600);
}

long microsecondsToCentimetres(long microseconds)
{
  return microseconds / 29 / 2;
}

long getDistance(int pin)
{
  long duration, cm;
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin,LOW);
  delayMicroseconds(2);
  digitalWrite(pin,HIGH);
  delayMicroseconds(5);
  digitalWrite(pin,LOW);
  
  pinMode(pin, INPUT);
  duration = pulseIn(pin, HIGH);  
  return microsecondsToCentimetres(duration);
}

OneWire ow(tempPin);

void lookUpSensors(int pin)
{
  byte address[8];
  int i;
  byte ok = 0;
  byte tmp = 0;
  
  Serial.println("-- Search Started --");
  while(ow.search(address)) {
    if (address[0] == 0x10) {
      Serial.println("Device is DS18S20");
      tmp = 1;
    }
    else if (address[0] == 0x28) {
      Serial.println("Device is DS18B20");
      tmp = 1;
    }
    if (tmp == 1) {
      if (OneWire::crc8(address,7) != address[7]) {
        Serial.println("CRC no valid");
      }
      else {
        for(i=0; i<8; i++) {
          if (address[i] < 9) {
            Serial.print("0");
          }
          Serial.print(address[i],HEX);
          if (i < 7) {
            Serial.print("-");
          }
          Serial.println("");
          ok = 1;
        }
      }
    }
  }
}

byte tempSensor[8] = {0x28,0x55,0xDF,0x84,0x02,0x00,0x00,0x72};

void writeToScratchpad(byte *address)
{
  ow.reset();
  ow.select(address);
  ow.write(0x44,1);
  delay(1000);
}

void readFromScratchpad(byte *address, byte *data)
{
  ow.reset();
  ow.select(address);
  ow.write(0xBE,1);
  for(int i=0; i<9; i++) {
    data[i] = ow.read();
  }
}

float getTemperature(byte *address)
{
  int tr;
  byte data[12];
  
  writeToScratchpad(address);
  readFromScratchpad(address,data);
  
  Serial.print("dyte[0]=");
  Serial.println(data[0], BIN);
  
  Serial.print("data[1]=");
  Serial.println(data[1], BIN);
  
  
  byte fraction = data[0] & 0x0F;
  byte number   = ((data[1] & 0x08) << 7) | (data[0] & 0xF0);
  if (data[1] > 0x80) {
    number = !number;
  }
  Serial.print("Number: ");
  Serial.println(number, BIN
  );
  
  
  return ((data[1] << 8) + data[0])*0.0625;
  
  
}

void loop()
{
  long cm = getDistance(pingPin);
  
  // Serial.print(", Centimetres: ");
  // Serial.println(cm);
  
  // lookUpSensors(tempPin);

  float temp = getTemperature(tempSensor);  
  Serial.print("Temp: ");
  Serial.println(temp);
  
  delay(2000);
}



