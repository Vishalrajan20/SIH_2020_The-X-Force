#include <stdlib.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h> 

#ifndef STASSID
#define STASSID "honor_9i"
#define STAPSK  "coolline"
#define FIREBASE_HOST "h2opointer.firebaseio.com"
#define FIREBASE_AUTH "mayzzGPa38qyD2gGcO1qzAlakK"
#endif


const char *ssid = STASSID;
const char *password = STAPSK;

SoftwareSerial nodemcu(2,3);
 
#define SensorPin A0          //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
 
// for float value to string converstion
int f;
  float val; // also works with double. 
  char buff2[10];
  String valueString = "";
  String Value = ""; 
  
void setup()
{
  pinMode(13,OUTPUT);  
  Serial.begin(9600);  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //nodemcu.begin(9600); 
}
void loop()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  
     Value =  dtostrf(phValue, 4, 2, buff2);  //4 is mininum width, 6 is precision
   valueString = valueString + Value +","; 
   Serial.println("9" + valueString);
   //nodemcu.println(valueString);
   Firebase.setInt("Ph",phValue);

  if (Firebase.failed()) { 
     Serial.print("setting /number failed:");    
}
  valueString = "";
 delay(3000);
 
}
