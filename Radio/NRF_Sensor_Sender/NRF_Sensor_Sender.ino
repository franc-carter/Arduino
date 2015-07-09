 


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "DHT.h"

#define DHTPIN 6    // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(8,7);

// Topology
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio pipe addresses for the 2 nodes to communicate.

int PIRpin = 5;
int TEMPpin = 6;

void setup()
{
  Serial.begin(57600);

  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(15,15);                 // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // 3 bytes, 1 byte for pin, 2 bytes for value
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  
  pinMode(PIRpin, INPUT);
  digitalWrite(PIRpin, HIGH);
}

int sendPacket(byte *data, int maxTries)
{
    int n = 0;
    while(n++ < maxTries) {
        if (radio.write(data, 1))
            return 1;
        delay(1);     
    }
    return 0;
}

//digitalRead(PIRpin)

void loop(void) {
    byte gotByte;  
    unsigned long time = micros();
    byte motion; 
  
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    Serial.println(h);
    Serial.println(t);
    
    motion = (byte)digitalRead(PIRpin);   

    Serial.println(motion);
    
    if (sendPacket(&motion,20)) {
      if(!radio.available()) { 
        printf("Blank Payload Received\n\r"); 
      } else {
        while(radio.available()) {
          unsigned long tim = micros();
          radio.read(&gotByte, 1);
          printf("Got response %d, round-trip delay: %lu milliseconds\n\r",gotByte,(tim-time)/1000+1);
        }
      }
    }
    else {
      Serial.print("failed\n");
    }
    delay(100);
}

