#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h> 

#ifndef STASSID
#define STASSID "honor_9i"
#define STAPSK  "coolline"
#define FIREBASE_HOST "astuteppm.firebaseio.com"
#define FIREBASE_AUTH "mayzzGPa38qyD2gGcO1qzAlakK7bzM1VrtuSmszh"
#endif


#define co2Zero 55 
#define motorpin1 D7
#define motorpin2 D8 
#define motorpin3 D5
#define motorpin4 D6 


const char *ssid = STASSID;
const char *password = STAPSK;

void setup() {
  
  
  pinMode(A0 , INPUT);                     //MQ135 analog feed set for input
  
  pinMode(motorpin1,OUTPUT);
    pinMode(motorpin2,OUTPUT); 
    pinMode(motorpin3,OUTPUT);
    pinMode(motorpin4,OUTPUT); 
 
  Serial.begin(115200);
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
 
}

void loop() {
  
int co2now[10];                               //int array for co2 readings
int co2raw = 0;                               //int for raw value of co2
int co2comp = 0;                              //int for compensated co2 
int co2ppm = 0;                               //int for calculated ppm
int zzz = 0;                                  //int for averaging
        



  for (int x = 0;x<10;x++){                   //samplpe co2 10x over 2 seconds
    co2now[x]=analogRead(A0);
    delay(200);
  }

for (int x = 0;x<10;x++){                     //add samples together
    zzz=zzz + co2now[x];
    
  }
  co2raw = zzz/10;                            //divide samples by 10
  co2comp = co2raw - co2Zero;                 //get compensated value
  co2ppm = map(co2comp,0,1023,4,500);      //map value for atmospheric levels


  Serial.print("PPM= ");
  
   Serial.print(co2ppm);
   Serial.println("  ");
   
   
Firebase.setInt("PPM",co2ppm);

if (Firebase.failed()) { 
     Serial.print("setting /number failed:");    
}
                         
  if(co2ppm>35){                            
    digitalWrite(motorpin1,HIGH);
digitalWrite(motorpin2,LOW);   

  }
    if(co2ppm>90){  
      digitalWrite(motorpin3,HIGH);  
    digitalWrite(motorpin4,LOW);                           
    }
    
    if(co2ppm<90){  
  digitalWrite(motorpin3,LOW);  
  digitalWrite(motorpin4,LOW);
  delay(2000);
  digitalWrite(motorpin1,HIGH);
  digitalWrite(motorpin2,LOW);   
                           
    }
}    
  
