

#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

#define SSID "<SSID>"
#define PASS "<PASSWD>"

#define ROOM "Study"

#define DOOR_PIN 1
#define DHT_PIN 2
#define PIR_PIN 0
#define PORT 7072

IPAddress  homectl(192,168,2,26);
WiFiClient client;
DHT dht(DHT_PIN, DHT22, 16);

Ticker tickerPIR;
Ticker tickerDHT;
Ticker tickerDOOR;

int doDHT  = 0;
int doPIR  = 0;
int doDOOR = 0;

void heartBeat(int doPin)
{
  switch (doPin) {
    case DOOR_PIN:
      doDOOR = 1;
      break;
    case DHT_PIN:
      doDHT = 1;
      break;
    case PIR_PIN:
      doPIR = 1;
      break;
  }
}

void setup()
{
  //Serial.begin(9600);

  pinMode(DOOR_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT_PULLUP);
  pinMode(DHT_PIN, INPUT_PULLDOWN);
  
  dht.begin();
  WiFi.begin(SSID, PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println(WiFi.localIP());

  tickerPIR.attach(60, heartBeat, PIR_PIN);
  tickerDOOR.attach(60, heartBeat, DOOR_PIN);
  tickerDHT.attach(60, heartBeat, DHT_PIN);
}

void setFHEM(const char *sensor)
{
  if (!client.connected())
    client.connect(homectl, PORT);
    
  client.print("set ");
  client.print(ROOM);
  client.print("_");
  client.print(sensor);
  client.print(" ");
}

static int prevDOOR = -1;
static int prevPIR = -1;

void loop()
{
  int pir = digitalRead(PIR_PIN);
  if (doPIR || pir != prevPIR) {
    setFHEM("PIR");
    const char *motion = (pir) ? "motion" : "still";
    client.println(motion);
    prevPIR = pir;
    doPIR = 0;
  }
    
  if (doDHT) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    setFHEM("Temperature"); client.println(t);
    setFHEM("Humidity"); client.println(h);
    doDHT = 0;
  }
    
  int door = digitalRead(DOOR_PIN);
  if (doDOOR || door != prevDOOR) {
    setFHEM("DOOR");
    const char *state = (door) ? "open" : "closed";
    client.println(state);
    prevDOOR = door;
    doDOOR = 0;
  }
  client.stop();
  yield();
}

