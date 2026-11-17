/*
  Week 6 - Graded Activity: Non-Blocking Robot Movement

  Learning objectives:
  - Run motor movement, an RGB blink, and a buzzer beep from three independent millis() timers
  - Confirm the robot moves continuously while the LED and buzzer keep their own separate rhythm

  Instructions:
  - The robot moves in a continuous circle using circleArc() - motors are never stopped
  - The RGB LED blinks blue every 500 ms
  - The buzzer beeps once every 1500 ms
  - Uncomment and fill in the blanks marked with _______
*/

////////////////////////////////////////////////////////////////////////////////
                              //CODE STARTS


// ================= LIBRARIES =================
#include <Wire.h>
#include <Adafruit_PCF8574.h>
#include <Adafruit_NeoPixel.h>

// ================= CONSTANTS =================
#define AIN1   A1
#define AIN2   A0
#define PWMA   6
#define BIN1   A2
#define BIN2   A3
#define PWMB   5

#define PIN         7    // RGB LED data line
#define BUZZER_PIN  5     // buzzer - PCF8574 pin P5

// ================= OBJECTS =================
Adafruit_PCF8574 pcf;
Adafruit_NeoPixel RGB = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);

// ================= TIMERS =================
unsigned long lastBlinkTime = 0;    // sticky note: when the LED last changed state
unsigned long lastBeepTime = 0;     // sticky note: when the buzzer last changed state
const unsigned long blinkInterval = 500;   // TODO: LED should blink every 500 ms
const unsigned long beepInterval = 1500;    // TODO: buzzer should beep every 1500 ms
bool ledState = false;      // tracks whether the RGB LED is currently ON or OFF
bool buzzerState = false;   // tracks whether the buzzer is currently sounding


// ================= MOVEMENT FUNCTION =================
// Example circleArc() from Week 3
void circleArc(int leftSpeed, int rightSpeed) {
  // Left motor
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, leftSpeed);

  // Right motor
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, rightSpeed);
}

// ================= SETUP =================
void setup() {
  // fill in the blank below, then initialise every peripheral this sketch uses
  pcf.begin(0x20, &Wire);            // TODO: PCF8574 I2C address
  pcf.pinMode(BUZZER_PIN, OUTPUT);
  pcf.digitalWrite(BUZZER_PIN, HIGH);   // active LOW, HIGH = silent

  RGB.begin();    // required once before any setPixelColor()/show() call works

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
}


// ================= LOOP =================
void loop() {
  // the robot keeps circling, this line runs on every single pass of loop()
  circleArc(60, 90);   
  // --- RGB LED Blink Task ---
  if (millis() - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = millis();
    ledState = !ledState;
    RGB.setPixelColor(0, ledState ? RGB.Color(0, 0, 255) : RGB.Color(0, 0, 0)); // Blue blink
    RGB.show();
  }

  // --- Buzzer Beep Task ---
  if (millis() - lastBeepTime >= beepInterval) {
    lastBeepTime = millis();
    buzzerState = !buzzerState;
    pcf.digitalWrite(BUZZER_PIN, buzzerState ? LOW : HIGH); // active LOW
  }
}