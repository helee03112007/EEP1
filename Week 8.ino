/*
  Week 8 - Activity: Live Sensor Pattern Monitoring

  Learning objectives:
  - Print the classification of all five sensors together, as one row, continuously
  - Visually connect the position of "B" in that row to which side the line is on

  Instructions:
  - Use your calibrated THRESHOLD from Week 7, Section 7
  - Uncomment and fill in the blanks marked with _______
*/

////////////////////////////////////////////////////////////////////////////////
                              //CODE STARTS


// ================= LIBRARIES =================
#include <IRArray.h>

// ================= CONSTANTS =================
#define THRESHOLD 500    // use your calibrated value from Week 7, Section 7

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

  for (int i = 0; i < 5; i++) {
    if (sensorValues[i] < THRESHOLD) {
      Serial.print("B ");
    } else {
      Serial.print("W");    // TODO: not black, so print "W " (note the trailing space, to keep columns aligned)
    }
  }
  Serial.println();

  delay(100);    // TODO: pick a short delay so the row updates fast enough to feel live, try 100
}