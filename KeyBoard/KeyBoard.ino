

#define KEYPADPIN A0

typedef struct keyInfo {
  int val;
  char key;
} keyInfo;

keyInfo keyArray[] = {
  {0, '\0'},
  {48,'1'} , {86,'2'},{129,'3'},
  {195,'4'},{309,'5'},{411,'6'},
  {546,'7'},{692,'8'},{779,'9'},
  {858,'*'},{921,'0'},{958,'#'},
  {1023,'\0'}
};

#define NKEYS       (sizeof(keyArray)/sizeof(keyInfo)-2)
#define LOWESTVAL   (keyArray[1].val/2)
#define LOWRANGE(i) (keyArray[(i)].val-((keyArray[(i)].val-keyArray[(i)-1].val)/2))
#define HIRANGE(i)  (keyArray[(i)].val+((keyArray[(i)+1].val-keyArray[(i)].val)/2))

char readKeyPad()
{
   char keyPressed = '\0';
   
   delay(50); // delay and re-read so value stabilises
   int pinVal = analogRead(KEYPADPIN);
   for(int i=1; i<=NKEYS; i++) {
     if (pinVal >= LOWRANGE(i) && pinVal < HIRANGE(i)) {
       keyPressed = keyArray[i].key;
       break;
     }
   }
   return keyPressed;
}

void setup()
{
  Serial.begin(9600);  //hardware serial to PC
}

void loop()
{
 int pinVal = analogRead(KEYPADPIN);
 if (pinVal >= LOWESTVAL) {
   char key = readKeyPad();
   if (key != '\0')
     Serial.println(key);
   while(analogRead(KEYPADPIN) >=LOWESTVAL)
     ;
 }                                            
}


