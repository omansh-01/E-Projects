//Including the libraries
#include<Servo.h>
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <Wire.h>

//Initializing the objects for servo motors  
Servo entryservo;
Servo exitservo;

//Initializing the variables
#define entry_servo D6
#define exit_servo D5
#define slot1 D0
#define slot2 D1
#define entryIR D2
#define exitIR D3
long duration, inches, cm;

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "S22Ultra"
#define WIFI_PASSWORD "12345678"

// Insert Firebase project API Key
#define API_KEY "AIzaSyD_YjtpOOLKgfTzPSiHeBCYj7WJIExnEn4"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "omanshbaranwal2001@gmail.com"
#define USER_PASSWORD "Time@24#12*01"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://smart-car-parking-system-83c6b-default-rtdb.asia-southeast1.firebasedatabase.app"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;
String slot_1;
String slot_2;
String entry_slot;

// IR sensors
float IR1;
float IR2;
float secs;
float entry_IR;


// Timer variables (send new readings every second)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 1000;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  Serial.println();
}

void setup(){
  Serial.begin(115200);
  pinMode(slot1,INPUT);
  pinMode(slot2,INPUT);
  pinMode(entryIR,INPUT);
  pinMode(exitIR,INPUT);
  entryservo.attach(entry_servo);
  exitservo.attach(exit_servo);
  entryservo.write(0);
  exitservo.write(0);  
  initWiFi();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
   Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);
  // Update database path
  databasePath = "test/";

  // Update database path for sensor readings
    slot_1 = databasePath + "slot1";
    slot_2 = databasePath + "slot2";
}
void sendstring(String path, String value){
      if (Firebase.RTDB.setString(&fbdo, path, value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
  }
void sendint(String path, int value){
      if (Firebase.RTDB.setInt(&fbdo, path, value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
  }

void loop(){
  entry_gate();
  exit_gate();
  // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
  }

    // Get latest sensor readings
    IR1 = digitalRead(slot1);
    IR2 = digitalRead(slot2);

    // Send readings to database:
    if ((IR1==1)&&(IR2==0)){
      sendstring(slot_1, "Empty");
      sendstring(slot_2, "Occupied");  
      }
    else if ((IR2==1)&&(IR1==0)){
      sendstring(slot_2, "Empty");
      sendstring(slot_1, "Occupied"); 
      }
    else if ((IR1==0)&&(IR2==1)){
      sendstring(slot_2, "Empty");
      sendstring(slot_1, "Occupied");
      }
    else if ((IR2==0)&&(IR1==1)){
      sendstring(slot_1, "Empty");
      sendstring(slot_2, "Occupied");
      }
    else if ((IR2==0)&&(IR1==0)){
      sendstring(slot_1, "Occupied");
      sendstring(slot_2, "Occupied");
      }
    else{
      sendstring(slot_1, "Empty");
      sendstring(slot_2, "Empty");
      }
  entry_gate();
  exit_gate();
}
  

void entry_gate(){
  int entrygate=digitalRead(entryIR);
  if (entrygate==0){
    entry_open();
    }
  
  else{
    entry_close();
    }
}
void exit_gate(){
  int exitgate=digitalRead(exitIR);
  if (exitgate==0){
    exit_open();
    }
  else{
    exit_close();
    }
  }
void entry_close(){
  entryservo.write(60);
  }
void entry_open(){
  entryservo.write(180);
  }
void exit_close(){
  exitservo.write(30);
  }
void exit_open(){
  exitservo.write(120);
  } 
