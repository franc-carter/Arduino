
/* YourDuinoStarter Example: nRF24L01 Receive Joystick values

 - WHAT IT DOES: Receives data from another transceiver with
   2 Analog values from a Joystick or 2 Potentiometers
   Displays received values on Serial Monitor
 - SEE the comments after "//" on each line below
 - CONNECTIONS: nRF24L01 Modules See:
 http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
   
 - V1.00 11/26/13
   Based on examples at http://www.bajdi.com/
   Questions: terry@yourduino.com
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
int joystick[2];  // 2 element array holding Joystick readings

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();;
}

void loop()
{
  if (radio.available()) {
    bool done = false;
    while (!done) {
      done = radio.read( joystick, sizeof(joystick) );
      Serial.print("X = ");
      Serial.print(joystick[0]);
      Serial.print(" Y = ");      
      Serial.println(joystick[1]);
    }
  }
  else    
      Serial.println("No radio available");

}

