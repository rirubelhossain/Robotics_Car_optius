/////////Varible//////////////
int sen[7], senPin[7] = {A8,A9,A10,A11,A12,A13,A14}; //Sensor Variable

int lastSpeedL, lastSpeedR, speed = 130;

int ref[7] = {702, 560, 570, 550, 560,  510, 750}; //Variable for Reference Value


//////////motor driver////////////
const int inA = 8, inB = 9, inC = 11, inD = 10;

//=================Sonar Pin========{tigr, echo}=======
int leftSonarPin[2] = {29, 27}, forwardSonarPin[2] = {33, 31}, rightSonarPin[2] = {37, 35};
float leftSonar, forwardSonar, rightSonar;


const int buttonPin = 52;

void setup() 
{
  for(int i = 0;i < 7; i++)
    pinMode(senPin[i],INPUT);
  
  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(inC, OUTPUT);
  pinMode(inD, OUTPUT);
  
  pinMode(buttonPin, INPUT);
  while (!digitalRead(buttonPin))
    delay(10);
  motor(100,100);
  delay(50);

  
  Serial.begin(9600);
}

/////////////////////// main loop /////////////////
void loop() {
  
   
  forwardSonar = sonarRead(forwardSonarPin[0], forwardSonarPin[1]);
  if(forwardSonar != 0 && forwardSonar <= 7){
      delay(10);
      forwardSonar = sonarRead(forwardSonarPin[0], forwardSonarPin[1]);
        if(forwardSonar != 0 && forwardSonar <= 7 ){
          brk(100);
          while(forwardSonar != 0 && forwardSonar <= 7){
            forwardSonar = sonarRead(forwardSonarPin[0], forwardSonarPin[1]);
            motor(0,0);
            delay(2000);
          }
      }
  }
  else{
    sensorRead(); 
  
    //+++++++++++ checker 
    if(( sen[5] && sen[4] && sen[3] && sen[2] && sen[1] ) || 
       (!sen[5] && sen[4] && sen[3] && sen[2] && sen[1]) || 
       (sen[5] && sen[4] && sen[3] && sen[2] && !sen[1])) {
       delay(10);
       sensorRead();
       if(( sen[5] && sen[4] && sen[3] && sen[2] && sen[1] ) || 
       (!sen[5] && sen[4] && sen[3] && sen[2] && sen[1]) || 
       (sen[5] && sen[4] && sen[3] && sen[2] && !sen[1])) 
       {
          brk(100);
          leftSonar = sonarRead(leftSonarPin[0],leftSonarPin[1]);
          if(leftSonar != 0 && leftSonar <= 12){
            motor(speed,speed);
            delay(400);
            brk(100);
            delay(500);
            sen[3] = 1;
            while(sen[3] != 0){
              sensorRead();
              motor(speed, -speed);
            }
            
            while(sen[3] != 1){
              sensorRead();
              motor(speed, -speed);
            }
            brk(100);    
            delay(500);    
         }
          else{
            motor(speed,speed);
            delay(300);
            brk(100);
            delay(500);
            sen[3] = 1;
            while(sen[3] != 0){
              sensorRead();
              motor(-speed, speed);
            }
            
            while(sen[3] != 1){
              sensorRead();
              motor(-speed, speed);
            }
            brk(100); 
            delay(500);
          }      
       }
    }

    //white sarface
    else if(!sen[5] && !sen[4] && !sen[3] && !sen[2] && !sen[1]){ 
     while( !sen[5] && !sen[4] && !sen[3] && !sen[2] && !sen[1]){
              sensorRead();
              brk(100);
              motor(0,0);
            }
     }

  
    else
    online();
  }
   
}


void online(){
      if(!sen[5] && !sen[4] && sen[3] && !sen[2] && !sen[1]){ 
        motor(speed,speed);
      }
      else if( !sen[5] && sen[4] && !sen[3] && !sen[2] && !sen[1] ){ 
        motor(speed,90);
      }
      else if(!sen[5] && !sen[4] && !sen[3] && sen[2] && !sen[1]){ 
        motor(90,speed);
      }
      else if( !sen[5] && sen[4] && sen[3] && !sen[2] && !sen[1]){ 
        motor(speed,100);
      }
      else if(!sen[5] && !sen[4] && sen[3] && sen[2] && !sen[1]){ 
        motor(100,speed);
      }
      else if(sen[5] && sen[4] && !sen[3] && !sen[2] && !sen[1] ){ 
        motor(speed,50);
      }
      else if(!sen[5] && !sen[4] && !sen[3] && sen[2] && sen[1] ){ 
        motor(50,speed);
      }
      else if(sen[5] && !sen[4] && !sen[3] && !sen[2] && !sen[1] ){ 
        motor(speed,50);
      }
      else if(!sen[5] && !sen[4] && !sen[3] && !sen[2] && sen[1] ){ 
        motor(50,speed);
      }
      else
        motor(100,100);
   }


////////////////Motor Driver///////////////////////////
void motor(int left, int right) {
  int a = 0, b = 0, c = 0, d = 0;
  lastSpeedL = left;
  lastSpeedR = right;

  if (left > 0)a = left;
  else b = left * -1;
  if (right > 0)c = right;
  else d = right * -1;
  analogWrite(inA, a);
  analogWrite(inB, b);
  analogWrite(inC, c);
  analogWrite(inD, d);
}

//break funtions
void brk(int a) {
  motor(-lastSpeedL, -lastSpeedR);
  delay(a);
  motor(0, 0);
}


///////sensor Read////////////////
void sensorRead() {
  //sensor calibration or refarance value determine
  /*
    for(int i=0; i<8; i++){
      Serial.print(i+1);
      Serial.print(" ");
      Serial.println(analogRead(senPin[i]));
    } Serial.println();Serial.println();
    return;
  */
  
  for (int i = 0; i < 7; i++) {
    sen[i] = analogRead(senPin[i]);
    if(sen[i] > ref[i])
      sen[i] = 1;
    else 
      sen[i] = 0;
 }
}
///////sensor Read End////////////////


//////////////sonar read////////////////////
long sonarRead(int trigPin, int echoPin){
  long duration, cm;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH, 2000);
  cm = duration / 58.2;
  return cm;
}
