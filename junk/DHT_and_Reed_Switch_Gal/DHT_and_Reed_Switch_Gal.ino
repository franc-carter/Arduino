
unsigned int t;

int doorPin[]     = {12,11, 10, 9};
int doorState[]    = { 0, 0, 0, 0};
int oldDoorState[] = { -1, -1, -1, -1};

#define nPins (sizeof(doorPin)/sizeof(int))

FILE *fp;

int check_interval = 100;  // (in milliseconds)
int report_interval = 1 * 10 * 1000; // report every 1 minutes

void setup(){
    delay(4000);
    Serial.begin(9600);
    for(int i=0; i<nPins; i++) {
      pinMode(doorPin[i], INPUT);
      digitalWrite(doorPin[i], HIGH);
    }
    fp = fopen("/var/spool/doors", "w");
}

void loop(){
  t = t + check_interval;
  if (t >= report_interval) {  // Device can't be read 'too' quickly
    for(int i=0; i<nPins; i++) {
        Serial.print("D"); Serial.print(i); Serial.print("="); Serial.println(doorState[i]);
        fprintf(fp, "D%d=%d\n", doorPin[i], doorState[i]);
    }
    fflush(fp);
    t = 0;
    //Serial.println("");
  }
  for(int i=0; i<nPins; i++) {
    doorState[i] = digitalRead(doorPin[i]);
    if (doorState[i] != oldDoorState[i]) {
      fprintf(fp, "D%d=%d\n", doorPin[i], doorState[i]);
      Serial.print("D"); Serial.print(i); Serial.print("="); Serial.println(doorState[i]);
      fflush(fp);
      oldDoorState[i] = doorState[i];
    }
  }
  delay(check_interval);
}

