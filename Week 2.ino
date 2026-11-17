/*
  Week 2 — Graded Activity: AlphaBot2 Command Console

  Learning objectives:
  - Read incoming serial commands using Serial.available() and Serial.read()
  - Use if/else statements to trigger different robot actions
  - Combine buzzer and LED control with serial input

  Commands to implement:
  - 'B' = trigger a single buzzer beep
  - 'R' = set all LEDs to Red
  - 'G' = set all LEDs to Green
  - 'M' = set all LEDs in sequence - Red, then Green, then Blue, then White
  - 'X' = turn all LEDs off and silence buzzer

  Instructions:
  - Uncomment lines in setup() to initialise all components
  - Fill in the blanks marked with _______ in loop()
*/


////////////////////////////////////////////////////////////////////////////////
                              //CODE STARTS


// ================= LIBRARIES =================
#include <Wire.h>
#include <Adafruit_PCF8574.h>
#include <Adafruit_NeoPixel.h>

// ================= CONSTANTS =================
#define BUZZER_PIN  5    // buzzer on PCF8574 pin 5
#define NUM_LEDS 4                  // Number of LED's on AlphaBot2
#define LED_PIN 7                   // data pin for the LED chain on AlphaBot2

// ================= OBJECTS =================
Adafruit_PCF8574 pcf;                                                            // PCF8574 object for buzzer
Adafruit_NeoPixel RGB = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); // NeoPixel object for LEDs

// ================= SETUP =================
void setup() {
  // start Serial communication at 9600 baud
  Serial.begin(9600);

  // initialise the PCF8574 expander
  pcf.begin(0X20, &Wire);
  pcf.pinMode(5, INPUT);           // Initialize Buzzer
  pcf.digitalWrite(BUZZER_PIN, HIGH);      // buzzer OFF at startup

  // initialise the LED strip
  RGB.begin();
  RGB.clear();
  RGB.show();    // turn all LEDs off at startup

  Serial.println("AlphaBot2 Ready. Commands: B=Buzz | R=Red | G=Green | M=Mixed | X=Off");
}


// ================= LOOP =================
void loop() {

  // check if a character has arrived from the Serial Monitor
  if (Serial.available() > 0) {

    // read the incoming character and store it in 'command'
    char command = Serial.read();

    // check which command was received and respond accordingly

    // if command is 'B' — beep the buzzer once
    if (command == 'B') {
      pcf.digitalWrite(5, INPUT);     // buzzer ON
      delay(200);
      pcf.digitalWrite(5, OUTPUT);    // buzzer OFF
      Serial.println("Buzzer activated!");
    }

    // if command is 'R' — set all 4 LEDs to Red
    else if (command == 'R') {
      RGB.setPixelColor(0, RGB.Color(255, 0, 0));    // LED 0
      RGB.setPixelColor(1, RGB.Color(255, 0, 0));    // LED 1
      RGB.setPixelColor(2, RGB.Color(255, 0, 0));    // LED 2
      RGB.setPixelColor(3, RGB.Color(255, 0, 0));    // LED 3
      RGB.show();
      Serial.println("LEDs set to Red");
    }

    // if command is 'G' — set all 4 LEDs to Green
    // TODO: Write this block yourself from scratch.
    // Use the 'R' block above as your reference for the pattern.
    else if (command == 'G') {
      RGB.setPixelColor(0, RGB.Color(0, 255, 0));    // LED 0
      RGB.setPixelColor(1, RGB.Color(0, 255, 0));    // LED 1
      RGB.setPixelColor(2, RGB.Color(0, 255, 0));    // LED 2
      RGB.setPixelColor(3, RGB.Color(0, 255, 0));    // LED 3
      RGB.show();
      Serial.println("LEDs set to Red");

    }

    // if command is 'M' — set all LEDs in sequence - Red, then Green, then Blue, then White
    // TODO: Write this block yourself from scratch.
    // Use the 'R' block above as your reference for the pattern.
    else if (command == 'M') {  
      RGB.setPixelColor(0, RGB.Color(255, 0, 0));    // LED 0
      RGB.setPixelColor(1, RGB.Color(0, 255, 0));    // LED 1
      RGB.setPixelColor(2, RGB.Color(0, 0, 255));    // LED 2
      RGB.setPixelColor(3, RGB.Color(255, 255, 255));    // LED 3
      RGB.show();
      Serial.println("LEDs set to Red");
      

    }

    // if command is 'X' — turn everything off
    else if (command == 'X') {
      RGB.clear();
      RGB.show();
      pcf.digitalWrite(BUZZER_PIN, HIGH);    // buzzer OFF
      Serial.println("All outputs off");
    }
  }
}
