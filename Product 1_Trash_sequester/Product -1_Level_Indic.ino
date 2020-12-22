#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h> 

#ifndef STASSID
#define STASSID "honor_9i"
#define STAPSK  "coolline"
#define FIREBASE_HOST "leveltest.firebaseio.com"
#define FIREBASE_AUTH "hw1oGy9KbgH0Fe8B3Qv3B9oPKXkgMUrN"
#endif

long duration1, distance1;
long duration2, distance2;
long duration3, distance3;

char dummy[100];
char jsonData[100];


const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

#define trigPin1 D2

#define echoPin1 D3



#define trigPin2 D4

#define echoPin2 D5



#define trigPin3 D6

#define echoPin3 D7

#define trigPin4 D8 

#define echoPin4 D9
  
void handleRoot() {

  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  server.send(200, "text/html", jsonData);
   //server.send(200, "text/html", sam);

}

void handleNotFound() {
 
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);

}

void setup(void) {
  
    Serial.begin (115200);

  pinMode(trigPin1, OUTPUT);

  pinMode(echoPin1, INPUT);

   pinMode(trigPin2, OUTPUT);

  pinMode(echoPin2, INPUT);

   pinMode(trigPin3, OUTPUT);

  pinMode(echoPin3, INPUT);

   pinMode(trigPin4, OUTPUT);

  pinMode(echoPin4, INPUT);
  
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
  

  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 
}


void loop(){

  long duration1, distance1;

  digitalWrite(trigPin1, LOW);  // Added this line

  delayMicroseconds(2); // Added this line

  digitalWrite(trigPin1, HIGH);

  delayMicroseconds(10); // Added this line

  digitalWrite(trigPin1, LOW);

  duration1 = pulseIn(echoPin1, HIGH);

  distance1 = (duration1*0.034) / 2;
  
   if (distance1 >= 25 || distance1 <= 0){

    distance1= -1;


  }


long duration2, distance2;

  digitalWrite(trigPin2, LOW);  // Added this line

  delayMicroseconds(2); // Added this line

  digitalWrite(trigPin2, HIGH);

  delayMicroseconds(10); // Added this line

  digitalWrite(trigPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH);
  distance2= (duration2*0.034)/2 ;

   if (distance2 >= 25 || distance2 <= 0){

    distance2= -1;
  }


  long duration3, distance3;

  digitalWrite(trigPin3, LOW);  // Added this line

  delayMicroseconds(2); // Added this line

  digitalWrite(trigPin3, HIGH);

  delayMicroseconds(10); // Added this line

  digitalWrite(trigPin3, LOW);

  duration3 = pulseIn(echoPin3, HIGH);

  distance3= (duration3*0.034) / 2;



   if (distance3 >= 25 || distance3 <= 0){

      distance3= -1;

  }
    long duration4, distance4;

  digitalWrite(trigPin4, LOW);  // Added this line

  delayMicroseconds(2); // Added this line

  digitalWrite(trigPin4, HIGH);

  delayMicroseconds(10); // Added this line

  digitalWrite(trigPin4, LOW);

  duration4 = pulseIn(echoPin4, HIGH);

  distance4 = (duration3*0.034) / 2;



   if (distance4 >= 25 || distance4 <= 0){

      distance4 = -1;

  }
  
  delay(3000);
  
sprintf(jsonData, "{plasticCanLevel:%ld, bioCanLevel:%ld, nonBioCanLevel:%ld, mixedCanLevel:%ld}\0",distance4, distance3, distance2, distance1);

Serial.write(jsonData);
Serial.println();


  for ( int i = 0; Serial.available() > 0; i++) {
    dummy[i] = Serial.read();
    Serial.println(dummy);
  }
  memset(jsonData, 0, sizeof(jsonData));
  strcpy(jsonData, dummy);
  server.handleClient();

Firebase.setInt("Plastic Can",distance4);
Firebase.setInt("Bio Can",distance3);
Firebase.setInt("Non-bio Can",distance2);
Firebase.setInt("Mixed Can",distance1);

if (Firebase.failed()) { 
     Serial.print("setting /number failed:"); 
     Serial.println(Firebase.error());   
     return; 
 }
delay(1000); 

}
