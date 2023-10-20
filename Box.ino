#include <FastLED.h>

FASTLED_USING_NAMESPACE

#include <Keypad.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>
#include<Servo.h>

#define DATA_PIN    10
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    60
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
Servo servo1;
Servo servo2;

LiquidCrystal liquid_crystal_display(9, 8, 7, 6, 5, 4);

char password[4];
char initial_password[4], new_password[4];
int i = 0;
int k = 0;
int p = 0;

int ldr = A5; //Set A0(Analog Input) for LDR
int value = 0;


int buzzer = 11;

int myservo1 = 12;
//int myservo2 = 13;

//int trigPin = 10;
//int echoPin = 3;
//long duration, inches, cm;

char key_pressed = 0;
const byte rows = 4;
const byte columns = 3;
char hexaKeys[rows][columns] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte row_pins[rows] = {A0, A1, A2, A3};
byte column_pins[columns] = {A4, 13, 2};
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void setup()
{

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
  Serial.begin(9600);

  //pinMode(trigPin, OUTPUT);

  //pinMode(echoPin, INPUT);

  pinMode(buzzer, OUTPUT);

  liquid_crystal_display.begin(16, 2);

  liquid_crystal_display.print("Password Secured");

  //Serial.print("Password");

  liquid_crystal_display.setCursor(0, 1);

  liquid_crystal_display.print("Box ");

  //Serial.print(" Secured Lock ");

  delay(1000);

  liquid_crystal_display.clear();

  liquid_crystal_display.print("Enter Password: ");

  //Serial.println();

  //Serial.println("Enter Password: ");

  liquid_crystal_display.setCursor(0, 1);

  initialpassword();

  servo1.attach(myservo1);

  //servo2.attach(myservo2);

  servo1.write(90);

  //servo2.write(90);

  //boxclose();
}




void loop()
{
  value = analogRead(ldr); //Reads the Value of LDR(light)
  Serial.println(value);
  key_pressed = keypad_key.getKey();



  if (key_pressed == '#')

    change();
  if (key_pressed == '*')

  {
    boxclose();
  }
  

  if (key_pressed)

  {

    password[i++] = key_pressed;

    liquid_crystal_display.print(key_pressed);

    //Serial.print(key_pressed);

  }
  //distance();
  if (i == 4)

  {

    delay(200);

    for (int j = 0; j < 4; j++) {

      initial_password[j] = EEPROM.read(j);
    }

    if (!(strncmp(password, initial_password, 4)))

    {

      liquid_crystal_display.clear();

      liquid_crystal_display.print("Password");

      liquid_crystal_display.setCursor(0, 1);

      liquid_crystal_display.print("Accepted");

      //Serial.println();

      //Serial.println("Pass Accepted");
      delay(1000);

      liquid_crystal_display.clear();

      liquid_crystal_display.print("Opening the box");

      //Serial.println("Opening the door");

      boxopen();

      delay(2000);

      liquid_crystal_display.clear();

      //liquid_crystal_display.clear();

      //liquid_crystal_display.print("Enter Password: ");

      //Serial.println("Enter Password: ");

      //liquid_crystal_display.setCursor(0, 1);

      i = 0;





    }

    else

    {
      k = k + 1;

      liquid_crystal_display.clear();

      liquid_crystal_display.print("Wrong Password");

      //Serial.println();

      //Serial.println("Wrong Password");

      tone(buzzer, 2048);

      digitalWrite(buzzer, HIGH);

      liquid_crystal_display.setCursor(0, 1);

      delay(1000);

      digitalWrite(buzzer, LOW);

      noTone(buzzer);

      delay(2000);

      liquid_crystal_display.clear();

      liquid_crystal_display.print("Enter Password");

      //Serial.println("Enter Password:");

      liquid_crystal_display.setCursor(0, 1);

      i = 0;
    }
    if (k == 3) {
      liquid_crystal_display.clear();
      boxclose();
      tone(buzzer, 2048);
      digitalWrite(buzzer, HIGH);
      liquid_crystal_display.print("Too Many");
      liquid_crystal_display.setCursor(0, 1);
      liquid_crystal_display.print("Wrong Attempts");
      delay(2000);
      liquid_crystal_display.clear();
      liquid_crystal_display.print("Retry in");
      liquid_crystal_display.setCursor(0, 1);
      liquid_crystal_display.print("One Minute");
      delay(5000);
      digitalWrite(buzzer, LOW);
      noTone(buzzer);
      delay(60000);
    }

  }


lights();
}
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void change()

{

  int j = 0;

  liquid_crystal_display.clear();

  liquid_crystal_display.print("Enter the");

  liquid_crystal_display.setCursor(0, 1);

  liquid_crystal_display.print("Current Password");

  delay(1000);

  //Serial.println("Current Password:");

  liquid_crystal_display.clear();

  liquid_crystal_display.print("Current Password");

  liquid_crystal_display.setCursor(0, 1);

  while (j < 4)

  {

    char key = keypad_key.getKey();

    if (key)

    {

      new_password[j++] = key;

      liquid_crystal_display.print(key);



    }

    key = 0;

  }

  delay(500);




  if ((strncmp(new_password, initial_password, 4)))

  {

    liquid_crystal_display.clear();

    liquid_crystal_display.print("Wrong Password");

    tone(buzzer, 2048);

    digitalWrite(buzzer, HIGH);

    delay(1000);

    digitalWrite(buzzer, LOW);

    noTone(buzzer);

    //Serial.println("Wrong Password");

    liquid_crystal_display.setCursor(0, 1);

    liquid_crystal_display.print("Try Again");

    //Serial.println("Try Again");

    delay(1000);

  }

  else

  {

    j = 0;

    liquid_crystal_display.print("Enter the");

    liquid_crystal_display.setCursor(0, 1);

    liquid_crystal_display.print("New Password");

    //Serial.println("New Password:");

    delay(1000);

    liquid_crystal_display.clear();

    liquid_crystal_display.print("New Password");

    liquid_crystal_display.setCursor(0, 1);

    while (j < 4)

    {

      char key = keypad_key.getKey();

      if (key)

      {

        initial_password[j] = key;

        liquid_crystal_display.print(key);

        EEPROM.write(j, key);

        j++;



      }

    }

    liquid_crystal_display.print("Password Changed");

    //Serial.println("Pass Changed");

    delay(1000);

  }

  liquid_crystal_display.clear();

  liquid_crystal_display.print("Enter Password: ");

  //Serial.println("Enter Password:");

  liquid_crystal_display.setCursor(0, 1);

  key_pressed = 0;

}




void initialpassword() {

  EEPROM.write(0, 50);
  EEPROM.write(1, 52);
  EEPROM.write(2, 53);
  EEPROM.write(3, 57);

  for (int j = 0; j < 4; j++){

    initial_password[j] = EEPROM.read(j);
    Serial.println(initial_password[j]);
}
}

void clear(char x[4]) {
  for (int i = 0; i < 4; i++) {
    x[i] = 0;
  }
}

/*
long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
*/
void clockwise1() {
  servo1.write(86);
  delay(1000);
  servo1.write(90);
  delay(5000);
}

void anticlockwise1() {
  servo1.write(97);
  delay(1000);
  servo1.write(90);
  delay(5000);
}

/*
  void clockwise2(){
  servo2.write(86);
  delay(1000);
  servo2.write(90);
  delay(1000);
  }

  void anticlockwise2(){
  servo2.write(97);
  delay(1000);
  servo2.write(90);
  delay(1000);
  }
*/
void boxopen() {
  clockwise1();
}

void boxclose() {
  anticlockwise1();
}
/*
void distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
}
*/
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void lights(){
// Call the current pattern function once, updating the 'leds' array
  bpm();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

}
