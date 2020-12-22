 #include <Stepper.h> 
 #include <Servo.h>
 #define STEPS 512
 Servo myservo; 
 int pos = 0;
const int stepsPerRevolution1 = 35;
const int stepsPerRevolution2 = 75;
int in1Pin = 12;
int in2Pin = 11;
int in3Pin = 10;
int in4Pin = 9;
Stepper myStepper(STEPS, in1Pin, in2Pin, in3Pin, in4Pin);
const int trigPin = 5;
const int echoPin = 6;
const int capSwitch = 4;
const int indSwitch = 2;
long duration;
float distance;
int ind, cap;

void setup() {
   myservo.attach(7);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(capSwitch,INPUT);
  pinMode(indSwitch,INPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  Serial.begin(9600);
  myStepper.setSpeed(10);
  
}

void servomotor()
{
    Serial.println("ser_start");
    for (pos = 60; pos >= 0; pos -= 1) 
     { 
    myservo.write(pos);             
    delay(15);                      
     } 
    delay(200);  
     for (pos = 0; pos <= 60; pos += 1) 
      {                                                                
    myservo.write(pos);             
    delay(15); 
     Serial.println("ser_stop");                      
      }
                              
  
}

void ultrasonic()
  {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(50);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  }
  
void stp_frwd()
{
  Serial.println("stp_start");
  myStepper.step(stepsPerRevolution2);
}

void stp_rev()
{
  myStepper.step(-stepsPerRevolution2);
  Serial.println("stp_done"); 
}

void stp_frwd2()
{
  Serial.println("stp_start");
  myStepper.step(stepsPerRevolution1);
}

void stp_rev2()
{
  myStepper.step(-stepsPerRevolution1);
  Serial.println("stp_done"); 
}


void loop()
{
    stp_frwd();
    delay(1000);
    servomotor();
    stp_rev();
    delay(1000);
/*
 ind = digitalRead(indSwitch);
 cap = digitalRead(capSwitch);
 ultrasonic();
 
if(distance < 3)
{    
  if(ind == LOW)
  {
    Serial.println("Non-Biodegradable object");
    Serial.println(ind);
    Serial.println(cap);
    Serial.println(distance);
    stp_frwd();
    delay(1000);
    stp_rev();
    delay(1000);
  }
  
  else if((cap != HIGH) && (ind == HIGH))
  {
    Serial.println("Biodegradable object");
    Serial.println(ind);
    Serial.println(cap);
    Serial.println(distance);
    stp_frwd();
    delay(1000);
    stp_rev();
    delay(1000);
  }
  else if((cap == HIGH) && (ind == HIGH))
  {
    Serial.println("plastic");
    Serial.println(ind);
    Serial.println(cap);
    Serial.println(distance);
    stp_frwd2();
    delay(1000);
    stp_rev2();
    delay(1000);
  }
}
*/
}
