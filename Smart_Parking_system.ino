 //TECHATRONIC.COM  
 // BLYNK LIBRARY  
 // https://github.com/blynkkk/blynk-library  
 // ESP8266 LIBRARY  
 // https://github.com/ekstrand/ESP8266wifi  
 #define TRIGGER D0  
 #define ECHO  D2  
 // NodeMCU Pin D0 > TRIGGER | Pin D2 > ECHO  
 #define BLYNK_PRINT Serial  // Comment this out to disable prints and save space  
 #include <ESP8266WiFi.h>  
 #include <BlynkSimpleEsp8266.h>  
 // You should get Auth Token in the Blynk App.  
 // Go to the Project Settings (nut icon).  
 char auth[] = "G0cmJMFEuBvXgCHdZklox-a8RVSN_YgF"; //Auth code sent via Email  
 // Your WiFi credentials.  
 // Set password to "" for open networks.  
 char ssid[] = "Maulik";  //Wifi name  
 char pass[] = "PCM12345"; //Wifi Password  
 void setup() {  
  Serial.begin (9600);  
  Blynk.begin(auth, ssid, pass);  
  pinMode(TRIGGER, OUTPUT);  
  pinMode(ECHO, INPUT);  
  pinMode(BUILTIN_LED, OUTPUT);  
 }  
 void loop() {  
  long duration, distance;  
  digitalWrite(TRIGGER, LOW);   
  delayMicroseconds(2);   
  digitalWrite(TRIGGER, HIGH);  
  delayMicroseconds(10);   
  digitalWrite(TRIGGER, LOW);  
  duration = pulseIn(ECHO, HIGH);  
  distance = (duration/2) / 29.1;  
   if (distance <=200) {  
   Blynk.virtualWrite(V0, 255);  
 }  
  else {  
   Blynk.virtualWrite(V0, 0);  
  }  
  if (distance <= 35) {  
   Blynk.virtualWrite(V1, 255);  
 }  
  else {  
   Blynk.virtualWrite(V1, 0);  
  }  
   if (distance <= 30) {  
   Blynk.virtualWrite(V2, 255);  
 }  
  else {  
   Blynk.virtualWrite(V2, 0);  
  }  
   if (distance <= 25) {  
   Blynk.virtualWrite(V3, 255);  
 }  
  else {  
   Blynk.virtualWrite(V3, 0);  
  }  
   if (distance <= 20) {  
   Blynk.virtualWrite(V4, 255);  
 }  
  else {  
   Blynk.virtualWrite(V4, 0);  
  }  
  Serial.print(distance);  
  Serial.println("Centimeter:");  
  Blynk.virtualWrite(V5, distance);  
  delay(200);  
  Blynk.run();  
  Serial.print(distance);  
  Serial.println("Centimeter:");  
  Blynk.virtualWrite(V6, distance);  
  delay(100);  
  Blynk.run();  
 }  
