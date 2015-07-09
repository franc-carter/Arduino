
/* State Machine States */
#define STATE_LOW        1
#define STATE_FALLEN     2
#define STATE_START_LOW  4
#define STATE_RISEN      8
#define STATE_START_HIGH 16
#define STATE_HIGH       32

#define ENCODERPIN1 2
#define ENCODERPIN2 3
#define TIMERPIN    12

volatile int   timerPinState = LOW;
volatile int   Pin1State     = STATE_HIGH;
volatile int   Pin2State     = STATE_HIGH;
volatile long  N             = 0;

void printState(int state)
{
  switch (state) {
    case STATE_HIGH:
      Serial.print("STATE_HIGH"); break;
    case STATE_FALLEN:
      Serial.print("STATE_FALLEN"); break;
    case STATE_START_LOW:
      Serial.print("STATE_START_LOW"); break;
    case STATE_LOW:
      Serial.print("STATE_LOW"); break;
    case STATE_RISEN:
      Serial.print("STATE_RISEN"); break;
    case STATE_START_HIGH:
      Serial.print("STATE_START_HIGH"); break;
  }
}

void Pin1StateChange()
{
  int pin = digitalRead(ENCODERPIN1);

  if (pin == HIGH) { // Rising
    switch (Pin1State) {
      case STATE_FALLEN:
      case STATE_START_LOW:
        Pin1State = STATE_HIGH;
        break;
      case STATE_LOW:
        Pin1State = STATE_RISEN;
        break;
      default:
        break;
    }
  }
  else { // Falling
    switch (Pin1State) {
      case STATE_HIGH:
        Pin1State = STATE_FALLEN;
        break;
      case STATE_RISEN:
      case STATE_START_HIGH:
        Pin1State = STATE_LOW;
        break;
      default:
        break;
    }
  }
  PORTC = Pin1State;
}

void Pin2StateChange()
{
  int pin = digitalRead(ENCODERPIN2);

  if (pin == HIGH) { // Rising
    switch (Pin2State) {
      case STATE_FALLEN:
      case STATE_START_LOW:
        Pin2State = STATE_HIGH;
        break;
      case STATE_LOW:
        Pin2State = STATE_RISEN;
        break;
      default:
        break;
    }
  }
  else { // Falling
    switch (Pin2State) {
      case STATE_HIGH:
        Pin2State = STATE_FALLEN;
        break;
      case STATE_RISEN:
      case STATE_START_HIGH:
        Pin2State = STATE_LOW;
        break;
      default:
        break;
    }
  }
}

/* 
 * Install the Interrupt Service Routine (ISR) for Timer2 overflow. 
 * This is normally done by writing the address of the ISR in the 
 * interrupt vector table but conveniently done by using ISR()
 */  
ISR(TIMER2_OVF_vect) {

  timerPinState = timerPinState == LOW ? HIGH : LOW;
  digitalWrite(TIMERPIN, timerPinState);

  switch (Pin1State) {
    case STATE_FALLEN:
      Pin1State = STATE_START_LOW;
      break;
    case STATE_START_LOW:
      Pin1State = STATE_LOW;
      if (Pin2State == STATE_LOW)
        N++;
      break;
    case STATE_RISEN:
      Pin1State = STATE_START_HIGH;
      break;
    case STATE_START_HIGH:
      Pin1State = STATE_HIGH;
      break;
    default:
      break;
  }

  switch (Pin2State) {
    case STATE_FALLEN:
      Pin2State = STATE_START_LOW;
      break;
    case STATE_START_LOW:
      Pin2State = STATE_LOW;
      if (Pin1State == STATE_LOW)
        N--;
      break;
    case STATE_RISEN:
      Pin2State = STATE_START_HIGH;
      break;
    case STATE_START_HIGH:
      Pin2State = STATE_HIGH;
      break;
    default:
      break;
  }  
  
  /* Reload the timer */  
  TCNT2 = 0;   
}

void setup() {
  
  
  Serial.begin (9600);

  pinMode(ENCODERPIN1, INPUT); 
  pinMode(ENCODERPIN2, INPUT);
  pinMode(TIMERPIN, OUTPUT);

  pinMode(14, OUTPUT); digitalWrite(14, LOW);
  pinMode(15, OUTPUT); digitalWrite(15, LOW); 
  pinMode(16, OUTPUT); digitalWrite(16, LOW);
  pinMode(17, OUTPUT); digitalWrite(17, LOW);
  pinMode(18, OUTPUT); digitalWrite(18, LOW);
  pinMode(19, OUTPUT); digitalWrite(19, LOW);

  digitalWrite(ENCODERPIN1, HIGH); //turn pullup resistor on
  digitalWrite(ENCODERPIN2, HIGH); //turn pullup resistor on

  attachInterrupt(0, Pin1StateChange, FALLING); 
  attachInterrupt(1, Pin2StateChange, FALLING);

  /*****  Timer Interrupt *****/

  /* First disable the timer overflow interrupt while we're configuring */  
  TIMSK2 &= ~(1<<TOIE2);  
  
  /* Configure timer2 in normal mode (pure counting, no PWM etc.) */  
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));  
  TCCR2B &= ~(1<<WGM22);  
  
  /* Select clock source: internal I/O clock */  
  ASSR &= ~(1<<AS2);  
  
  /* Disable Compare Match A interrupt enable (only want overflow) */  
  TIMSK2 &= ~(1<<OCIE2A);  
  
  /* Configure the prescaler */
  TCCR2B |= (1<<CS22);
  TCCR2B |= (1<<CS21);
  TCCR2B &= ~(1<<CS20);  

  /* Finally load end enable the timer */  
  TCNT2 = 0;
  TIMSK2 |= (1<<TOIE2);  
  
  Serial.begin(9600);
}

void loop() {
  Serial.print("Pin1State="); printState(Pin1State);
  Serial.print(" Pin2State="); printState(Pin2State);
  Serial.print(" N="); Serial.println(N);
  Serial.println(PORTC);
  delay(100);
} 


