/*
  Week 6 - Activity: Hardware Interrupt

  Learning objectives:
  - Attach a hardware interrupt to pin D2 on FALLING edge
  - Write a minimal ISR that toggles an LED
  - Observe that the interrupt fires instantly even during delay()

  Behaviour:
  - Onboard buzzer beeps once every 4 seconds continuously (2s ON, 2s OFF)
  - Press button once → external LED turns ON
  - Press button again → external LED turns OFF
  - External LED responds instantly at any point during the beep cycle
*/

////////////////////////////////////////////////////////////////////////////////
                              //CODE STARTS


// ================= LIBRARIES =================
#include <Wire.h>
#include <Adafruit_PCF8574.h>

// ================= PIN DEFINITIONS =================
#define BUZZER_PIN  5     // onboard buzzer - PCF8574 pin P5
#define EXT_LED     4     // external LED connected via jumper header
#define BUTTON_PIN  2     // interrupt pin - must be D2 or D3 on Arduino Uno

// ================= OBJECTS =================
Adafruit_PCF8574 pcf;

volatile bool ledState = false;   // tracks whether the external LED is ON or OFF

// ================= ISR =================
void onButtonPress() {
  if (ledState == false) {
    ledState = true;                 // update state to ON
    digitalWrite(EXT_LED, HIGH);     // turn external LED ON
  } else {
    ledState = false;                // update state to OFF
    digitalWrite(EXT_LED, LOW);      // turn external LED OFF
  }
}

// ================= SETUP =================
void setup() {
  pcf.begin(0x20, &Wire);         // PCF8574 I2C address
  pcf.pinMode(BUZZER_PIN, OUTPUT); // set as OUTPUT — this buzzer just beeps on its own
  pinMode(EXT_LED, OUTPUT);      // set as OUTPUT — this LED is toggled by the ISR
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // enable the internal pull-up (INPUT_PULLUP)

  // Call the onButtonPress() ISR when pin 2 is pulled down to Ground
  // fill in: (interrupt pin, ISR function name, trigger mode)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPress, FALLING);
}

// ================= LOOP =================
void loop() {
  pcf.digitalWrite(BUZZER_PIN, LOW);   // buzzer ON (active LOW)
  delay(2000);                         // buzzer ON for 2 seconds
  pcf.digitalWrite(BUZZER_PIN, HIGH);  // buzzer OFF
  delay(2000);                         // buzzer OFF for 2 seconds
}