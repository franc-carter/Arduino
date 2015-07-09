
#include <OneWire.h>
#include "DHT.h"

/*

Poll Hot Wate pipe 

*/


#define DHTPIN1        2
#define DHTTYPE        DHT11
#define HOTWATERPIN    7
#define HOTWATERPOLL   ((unsigned long)10000)
#define VIBRATIONPIN   A0
#define TEMPNOTIFY     0.5
#define REPORTINTERVAL ((unsigned long)60000)

DHT dht1(DHTPIN1, DHTTYPE);
OneWire onewire(HOTWATERPIN);

typedef struct {
    int pin;
    int state;
    int oldState;
} door_type;

door_type doors[] = {
    {12, 0, -1},
    {11, 0, -1},
    {10, 0, -1},
    { 9, 0, -1},
    { 8, 0, -1},
    { 6, 0, -1},
    { 5, 0, -1},
    { 4, 0, -1},
};

#define nPins (sizeof(doors)/sizeof(door_type))

#define printPin(type,pin,val) \
  Serial.print(type); \
  Serial.print(pin); \
  Serial.print("="); \
  Serial.println(val);

unsigned long prev_t = 0;
unsigned long t = 0;
unsigned long last_poll = 0;
unsigned long vibrationSum = 0;

int doHotWater(int forced)
{
  static float prevTemp = 1000.0;
  byte data[12], addr[8];
  
  last_poll = t;
  onewire.reset();
  if (!onewire.search(addr)) {
    onewire.reset_search();
    delay(250);
    return 0;
  }
  if (OneWire::crc8(addr, 7) != addr[7])
      return 0;
      
  onewire.reset();
  onewire.select(addr);
  onewire.write(0x44,1);         // start conversion, with parasite power on at the end
  delay(750); // max conversion time
  
  onewire.reset();
  onewire.select(addr);    
  onewire.write(0xBE);         // Read Scratchpad

  for ( int i = 0; i < 9; i++) // we need 9 bytes
    data[i] = onewire.read();
    
  // convert the data to actual temperature
  unsigned int raw = (data[1] << 8) | data[0];

  byte cfg = (data[4] & 0x60);
  if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
  
  float temp = (float)raw / 16.0;
  if ((abs(temp-prevTemp) > TEMPNOTIFY) || forced) {
    prevTemp = temp;
    printPin('T',HOTWATERPIN,temp);
  }
  else
    Serial.println("Ping");
  
  return 1;
}

void doDHT(DHT dht)
{
  int   pin = dht.pin();
  float h   = dht.readHumidity();
  float t   = dht.readTemperature();
  
  printPin('H',pin,h);
  printPin('T',pin,t);
}

void doDoors()
{
  for(int i=0; i<nPins; i++) {
    doors[i].state = digitalRead(doors[i].pin);
    if (doors[i].state != doors[i].oldState) {
      printPin('D',doors[i].pin,doors[i].state);
      doors[i].oldState = doors[i].state;
    }
  }

}

void setup()
{
    Serial.begin(9600);
    delay(2000);
    for(int i=0; i<nPins; i++) {
      pinMode(doors[i].pin, INPUT_PULLUP);
    }
    dht1.begin();
}

void loop()
{
  static int pingN = 0;
  t = millis();

  vibrationSum = vibrationSum + (unsigned long)analogRead(VIBRATIONPIN);

  if (t-prev_t >= REPORTINTERVAL) {
    prev_t = t;
    for(int i=0; i<nPins; i++) {
      printPin('D',doors[i].pin,doors[i].state);
    }
    printPin('A',0,vibrationSum);
    vibrationSum = 0;
    doDHT(dht1);
    doHotWater(TRUE);
  }
  if (t-last_poll >= HOTWATERPOLL)
    doHotWater(FALSE);
  doDoors();
}

