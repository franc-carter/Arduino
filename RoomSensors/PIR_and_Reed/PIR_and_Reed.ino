
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
    { 3, 0, -1},
    { 2, 0, -1},
};

#define nPins (sizeof(doors)/sizeof(door_type))

int check_interval = 100;  // (in milliseconds)
int report_interval = 5 * 60 * 1000; // report every 5 minutes

void setup()
{
    Serial.begin(9600);
    delay(4000);
    for(int i=0; i<nPins; i++) {
      pinMode(doors[i].pin, INPUT);
      digitalWrite(doors[i].pin, HIGH);
    }
}

void print_door(char *type, int i)
{
  Serial.print(type);
  Serial.print(doors[i].pin);
  Serial.print("=");
  Serial.println(doors[i].state);
}

void loop(){
  t = t + check_interval;
  if (t >= report_interval) {
    for(int i=0; i<nPins; i++)
        print_door("D", i);
    t = 0;
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

