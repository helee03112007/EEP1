/*
  Week 9 - Graded Activity
  AlphaBot2 Slotted Shaped Track Challenge
  PD Line Following
*/

#include <IRArray.h>

// ================= PIN DEFINITIONS =================
#define AIN1 A0
#define AIN2 A1
#define PWMA 6
#define BIN1 A3
#define BIN2 A2
#define PWMB 5

// ================= CONSTANTS =================
#define THRESHOLD  500
#define BASE_SPEED 70
#define MAX_SPEED  120

// ================= TUNING =================
// Replace these with your tuned values from Section 3
float Kp = 30.0;
float Kd = 400.0;

// ================= OBJECTS =================
IRArray ir;
unsigned int sensorValues[5];

// ================= VARIABLES =================
int weights[5] = {2, 1, 0, -1, -2};

float lastKnownError = 0;
float previousError = 0;

// ================= MOVEMENT FUNCTIONS =================
void moveForward(int leftSpeed, int rightSpeed) {
  // Left motor forward
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  // Right motor forward
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  analogWrite(PWMA, leftSpeed);
  analogWrite(PWMB, rightSpeed);
}

void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}

// ================= ERROR FUNCTION =================
float readError() {
  ir.read(sensorValues);

  float weightedSum = 0;
  int activeSensors = 0;

  for (int i = 0; i < 5; i++) {
    if (sensorValues[i] < THRESHOLD) {
      weightedSum += weights[i];
      activeSensors++;
    }
  }

  if (activeSensors > 0) {
    lastKnownError = weightedSum / activeSensors;
  }

  return lastKnownError;
}

// ================= SETUP =================
void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  ir.begin();

  stopMotors();
  delay(1000);
}

// ================= LOOP =================
void loop() {
  float error = readError();

  // Derivative: change in error between loops
  float derivative = error - previousError;

  // PD correction
  float correction = (Kp * error) + (Kd * derivative);

  // Adjust motor speeds in opposite directions
  int leftSpeed = BASE_SPEED - correction;
  int rightSpeed = BASE_SPEED + correction;

  // Keep speeds within valid PWM limits
  leftSpeed = constrain(leftSpeed, 0, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, 0, MAX_SPEED);

  // Move the robot
  moveForward(leftSpeed, rightSpeed);

  // Save error for the next loop
  previousError = error;
}