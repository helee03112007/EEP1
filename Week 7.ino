/*
  Week 7 — Graded Mini Project: Surface Detection System

  Learning objectives:
  - Combine five individual sensor classifications into one overall decision
  - Use a count-based rule to distinguish open surface, line, and edge

  Instructions:
  - Uncomment and fill in the blanks marked with _______
*/

////////////////////////////////////////////////////////////////////////////////
                              //CODE STARTS


// ================= LIBRARIES =================
#include <IRArray.h>

// ================= CONSTANTS =================
#define THRESHOLD 500    // use your calibrated value from Section 7

// ================= OBJECTS =================
IRArray ir;
unsigned int sensorValues[5];


// ================= SETUP =================
void setup() {

  Serial.begin(115200);
  ir.begin();
}


// ================= LOOP =================
void loop() {
  ir.read(sensorValues);

  int blackCount = 0;
  for (int i = 0; i < 5; i++) {
    if (sensorValues[i] < THRESHOLD) {
      blackCount = blackCount + 1;        // increase the count by 1 each time a sensor reads black
    }
  }

  if (blackCount == 0) {
    Serial.println("White region");             // no sensors see black, fully on White surface
  } else if (blackCount == 5) {            // TODO: all 5 sensors read black
    Serial.println("Black region");               // all sensors see black, fully on Black surface
  } else {
    Serial.println("Edge area");                   // TODO: some, but not all, see black — print "Edge area"
  }

  delay(200);
}