/*
  Week 5 Part A — Drive Straight Tuning

  Goal:
  - Run the robot forward for 2 seconds and observe if it drifts
  - Adjust LEFT_SPEED or RIGHT_SPEED by 4-5 PWM steps at a time
  - Re-upload and repeat until the robot goes straight

  How to tune:
  - Robot drifts RIGHT → right motor is slower → increase RIGHT_SPEED by 4 or 5
  - Robot drifts LEFT  → left motor is slower  → increase LEFT_SPEED by 4 or 5
  - Robot goes straight → done, note down your LEFT_SPEED and RIGHT_SPEED values
*/

////////////////////////////////////////////////////////////////////////////////
                              //CODE STARTS


// ================= PIN DEFINITIONS =================
#define AIN1  A1        // left motor direction pin 1
#define AIN2  A0        // left motor direction pin 2
#define PWMA  6         // left motor speed — PWM
#define BIN1  A2        // right motor direction pin 1
#define BIN2  A3        // right motor direction pin 2
#define PWMB  5         // right motor speed — PWM

// ================= SPEED CONSTANTS =================
// Start with both equal. Adjust one side by 4-5 PWM if the robot drifts.
// Example: if robot drifts right, change RIGHT_SPEED from 180 to 184 or 185
#define LEFT_SPEED   60   // increase this if robot drifts RIGHT
#define RIGHT_SPEED  60   // increase this if robot drifts LEFT


// ================= MOVEMENT FUNCTIONS =================
void moveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, leftSpeed);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, rightSpeed);
}

void stopMotors() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 0);
}


// ================= SETUP =================
void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // run forward for 2 seconds then stop
  moveForward(LEFT_SPEED, RIGHT_SPEED);
  delay(6000);
  stopMotors();
}


// ================= LOOP =================
void loop() {
  // nothing here — tuning happens in setup()
}