
#include "DHT.h"

#define DHTPIN1 2     // what pin we're connected to
// #define DHTPIN2 3     // what pin we're connected to

//#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define DHTTYPE DHT11   // DHT 22  (AM2302)

//DHT dht1(DHTPIN1, DHTTYPE);
// DHT dht2(DHTPIN2, DHTTYPE);

unsigned int t;

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
    { 7, 0, -1},
    { 6, 0, -1},
    { 5, 0, -1},
    { 4, 0, -1},
};

#define nPins (sizeof(doors)/sizeof(door_type))

int check_interval = 100;  // (in milliseconds)
int report_interval = 1 * 60 * 1000; // report every ? minutes

void setup()
{
    Serial.begin(9600);
    delay(4000);
    for(int i=0; i<nPins; i++) {
      pinMode(doors[i].pin, INPUT_PULLUP);
    }
    //dht1.begin();
    //dht2.begin();
}

void print_door(char *type, int i)
{
  Serial.print(type);
  Serial.print(doors[i].pin);
  Serial.print("=");
  Serial.println(doors[i].state);
}

void doDHT(DHT dht)
{
  int   pin = dht.pin();
  float h   = dht.readHumidity();
  float t   = dht.readTemperature();
  Serial.print("H");Serial.print(pin);Serial.print("="); 
  Serial.println(h);
  Serial.print("T");Serial.print(pin);Serial.print("="); 
  Serial.println(t);
}

void loop()
{
  t = t + check_interval;
  if (t >= report_interval) {
    for(int i=0; i<nPins; i++)
        print_door("D", i);
    t = 0;
//    doDHT(dht1);
//    doDHT(dht2);
  }
  for(int i=0; i<nPins; i++) {
    doors[i].state = digitalRead(doors[i].pin);
    if (doors[i].state != doors[i].oldState) {
      print_door("D", i);
      doors[i].oldState = doors[i].state;
    }
  }
  delay(check_interval);
}

