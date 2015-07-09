
#include <stdint.h>
#include <RCSwitch.h>

#define DIRECTION_PIN     4
#define STEP_PIN          3
#define STOP_PIN          8
#define ENABLE_PIN        7
#define MS1_PIN           6
#define MS2_PIN           5

#define SET_SPEED 1
#define SET_POS   2

#define STEPPER_ENABLE    0
#define STEPPER_DISABLE   1
#define CLOCKWISE         LOW
#define ANTICLOCKWISE     HIGH

#define STEP_MULTIPLIER 10;

typedef struct radio_msg_t {
  uint8_t device_id;
  uint8_t cmd;
  uint16_t value;
} radio_msg_t;

RCSwitch mySwitch      = RCSwitch();
int      myID          = 0x03;
int      crnt_position = 0;
int      step_delay    = 8000;

// grr - typedef scoping is broken in Arduino
radio_msg_t RadioMsg;

void set_stepper_enable(int enabled)
{
  digitalWrite(ENABLE_PIN, enabled);
  if (enabled == STEPPER_ENABLE)
    delay(10); // wait for everything to wake up
}

void move_steps(int n)
{
  set_stepper_enable(STEPPER_ENABLE);
  for (int i = 0; i<n; i++) {
    for(int j = 0; j<STEP_MULTIPLIER; j++) {
      digitalWrite(STEP_PIN, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(STEP_PIN, HIGH); // rising edge whuch triggers the step 
      delayMicroseconds(step_delay);
    }
  }
  set_stepper_enable(STEPPER_DISABLE);
}

void set_direction(int dir)
{
  digitalWrite(DIRECTION_PIN, dir);     // Set the direction.
  delay(100);
}

void move_to_end_stop()
{
  set_stepper_enable(STEPPER_ENABLE);
  set_direction(CLOCKWISE);
  int n = 0;
  while (digitalRead(STOP_PIN) == HIGH) { // inverted logic from INPUT_PULLUP
      digitalWrite(STEP_PIN, LOW);
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(step_delay);
      digitalWrite(STEP_PIN, LOW);
      n++;
  }
  set_direction(ANTICLOCKWISE);
  move_steps(20);
  set_stepper_enable(STEPPER_DISABLE);
  crnt_position = 0;
}

int get_radio_msg(void)
{
  int got_msg = 0;

  if (mySwitch.available()) {
    unsigned long raw = mySwitch.getReceivedValue();
    if (raw != 0) {
      RadioMsg.device_id = (raw & 0xf00000) >> 20;
      RadioMsg.cmd       = (raw & 0x0f0000) >> 16;
      RadioMsg.value     = (raw & 0x00ffff);
      got_msg            = 1; 
    }
  }
  mySwitch.resetAvailable();

  return got_msg; 
}

void do_cmd(uint8_t cmd, uint16_t value)
{
  switch (cmd) {
    case 0x01:
      Serial.print("Setting speed to "); Serial.println(value);
      step_delay = 10000 - (value * 100);
      break;
    case 0x02:
      Serial.print("Moving to "); Serial.println(value);
      int move = value - crnt_position;
      if (move != 0) {
        if (move < 0) {
          move = -move;
          set_direction(CLOCKWISE);
        }
        else
          set_direction(ANTICLOCKWISE);
        move_steps(move);
        crnt_position = value;
      }
      break;
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

  //move_to_end_stop();
  set_stepper_enable(STEPPER_DISABLE);
}

void loop()
{
  int found = get_radio_msg();
  if (found && RadioMsg.device_id == myID)
    do_cmd(RadioMsg.cmd, RadioMsg.value);
}

