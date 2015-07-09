

int StepsPerRev = 50;
int MicroStep = 4;
int Steps = 0;  // Record the number of steps we've taken

void setup() {                
  pinMode(6, OUTPUT);     
  pinMode(7, OUTPUT);

  pinMode(8, OUTPUT);     
  pinMode(9, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

//  No Movement
//  digitalWrite(6, LOW);
//  digitalWrite(7, LOW);

// 4 x 50 is two revolutions
// digitalWrite(6, LOW);
// digitalWrite(7, HIGH);

// 4 x 50 is four resolutions
// digitalWrite(6, HIGH);
// digitalWrite(7, LOW);

// 4 x 50 is a full revolution
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);


}

void loop() {
  digitalWrite(9, HIGH);
  delayMicroseconds(1000);          
  digitalWrite(9, LOW); 
  delayMicroseconds(1000);
  
  Steps = Steps + 1;   // record this step
  
  // Check to see if we are at the end of our move
  if (Steps == StepsPerRev*MicroStep) {
    // We are! Reverse direction (invert DIR signal)
    if (digitalRead(8) == LOW) {
      digitalWrite(8, HIGH);
    }
    else {
      digitalWrite(8, LOW);
    }

    // Reset our distance back to zero since we're
    // starting a new move
    Steps = 0;
    
    // Now pause for half a second
    delay(500);
  }
}

