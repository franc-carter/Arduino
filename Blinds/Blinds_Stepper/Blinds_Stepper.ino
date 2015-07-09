
#include <RCSwitch.h>

#define DIRECTION_PIN     9
#define STEP_PIN          8
#define STOP_PIN          3
#define ENABLE_PIN        4
#define SLEEP_PIN         6
#define MS1_PIN           7
#define MS2_PIN           5

#define SET_SPEED 1
#define SET_POS   2

#define STEPPER_ENABLE    0
#define STEPPER_DISABLE   1
#define CLOCKWISE         LOW
#define ANTICLOCKWISE     HIGH

RCSwitch mySwitch      = RCSwitch();
int      myID          = 1;
int      crnt_position = 0;
int      crnt_speed    = 8000;

void move_steps(int n, int stepdelay)
{
  for (int i = 0; i<n; i++) {
    for(int j = 0; j<10; j++) {
      digitalWrite(STEP_PIN, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(STEP_PIN, HIGH); // rising edge whuch triggers the step 
      delayMicroseconds(stepdelay);
    }
  }
}

void set_stepper_enable(int enabled)
{
  pinMode(ENABLE_PIN, enabled);
  if (enabled == STEPPER_ENABLE)
    delay(10); // wait for everything to wake up
}

void set_direction(int dir)
{
  digitalWrite(DIRECTION_PIN, dir);     // Set the direction.
  delay(100);
}

void move_to_end_stop(int stepdelay)
{
  set_stepper_enable(STEPPER_ENABLE);
  set_direction(CLOCKWISE);
  Serial.println("Moving to end stop");
  int n = 0;
  while (digitalRead(STOP_PIN) == HIGH) { // inverted logic from INPUT_PULLUP
//  while (0 < 1) {
      digitalWrite(STEP_PIN, LOW);
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(stepdelay);
      digitalWrite(STEP_PIN, LOW);
      n++;
  }
  Serial.println("Found end stop");
  Serial.println(n);
  set_direction(ANTICLOCKWISE);
  move_steps(20,8000);
  set_stepper_enable(STEPPER_DISABLE);
}

int read_new_position()
{
  int n = Serial.parseInt(); 
  
  Serial.read();
  return n;
}

void checkRadio()
{
  if (mySwitch.available()) {
    unsigned long msg = mySwitch.getReceivedValue();
    if (msg != 0) {
      unsigned long dstID = (msg & 0xf00000) >> 20;
      if (dstID == myID) {
        unsigned long cmd   = (msg & 0x0f0000) >> 16;
        unsigned long val   = (msg & 0x00ffff);
        switch (cmd) {
          case SET_SPEED:
            crnt_speed = val * 100;
            break;
          case SET_POS:
            int move = val - crnt_position;
            if (move != 0) {
              if (move < 0) {
                move = -move;
                set_direction(CLOCKWISE);
              }
            } else
              set_direction(ANTICLOCKWISE);
            move_steps(move,8000);
            crnt_position  = val;
            break;
        }
      }
    }
    mySwitch.resetAvailable();
  }
}

void setup() 
{
  Serial.begin(9600);
  
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(STOP_PIN, INPUT_PULLUP);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(MS1_PIN, OUTPUT);
  pinMode(MS2_PIN, OUTPUT);

  digitalWrite(MS1_PIN, LOW);
  digitalWrite(MS2_PIN, LOW);

  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2

  delay(2000);

  move_to_end_stop(8000);
  crnt_position = 0;
}

void loop()
{
  set_stepper_enable(STEPPER_DISABLE);
  
  checkRadio();

/*
  if (Serial.available()) {
    int new_position = read_new_position();
    if (new_position >= 0) {
      Serial.print("New Position: ");
      Serial.println(new_position);
      int move = new_position - crnt_position;
      Serial.print("Moving:");
      Serial.println(move);
      if (move != 0) {
        set_stepper_enable(STEPPER_ENABLE);
        if (move < 0) {
          move = -move;
          set_direction(CLOCKWISE);
        } else {
          set_direction(ANTICLOCKWISE);
        }
        move_steps(move,8000);
        crnt_position  = new_position;
        Serial.println("Done");
      }
    }
  }
*/
}

