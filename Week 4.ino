/*
  Week 4 — Graded Activity: Distance Based Speed Control

  Learning objectives:
  - Combine ultrasonic sensing with PWM motor control
  - Implement the sense-decide-act loop
  - Use map() and conditional logic to scale distance to motor speed

  Instructions:
  - Copy your moveForward() and stopMotors() functions from Week 3 above setup()
  - Fill in the trigger sequence inside readDistance()
  - Complete the speed logic inside loop()
  - Uncomment and fill in the blanks marked with _______
*/

////////////////////////////////////////////////////////////////////////////////
                              //CODE STARTS


// ================= PIN DEFINITIONS =================
#define TRIG_PIN  3     // HC-SR04 Trig — Arduino pin D3
#define ECHO_PIN  2     // HC-SR04 Echo — Arduino pin D2

#define AIN1  A1        // left motor direction pin 1
#define AIN2  A0        // left motor direction pin 2
#define PWMA  6         // left motor speed — PWM
#define BIN1  A2        // right motor direction pin 1
#define BIN2  A3        // right motor direction pin 2
#define PWMB  5         // right motor speed — PWM

// ================= THRESHOLDS =================
#define STOP_DISTANCE  15    // stop completely below this distance (cm)
#define SLOW_DISTANCE  30    // start slowing down below this distance (cm)
#define FULL_SPEED    200    // PWM value for full speed


// ================= MOVEMENT FUNCTIONS =================
// IMP - copy your moveForward() and stopMotors() from Week 3 here

void moveForward(int speed) {
  // left motor forward: AIN1 HIGH, AIN2 LOW
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  // right motor forward: BIN1 HIGH, BIN2 LOW
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  // apply speed to both motors
  analogWrite(PWMA, 200);
  analogWrite(PWMB, 200);
}

void moveBackward(int speed) {
  // left motor reverse: AIN1 LOW, AIN2 HIGH
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  // right motor reverse: BIN1 LOW, BIN2 HIGH
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  analogWrite(PWMA, 200);
  analogWrite(PWMB, 200);
}

void turnLeft(int speed) {
  // left motor stop (coast): both pins LOW, speed 0
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0);

  // right motor forward at given speed
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 0);
}

void turnRight(int speed) {
  // left motor forward at given speed
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 0);

  // right motor stop (coast): both pins LOW, speed 0
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 0);
}

void stopMotors() {
  // both motors coast: all direction pins LOW, speed 0
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}


// ================= DISTANCE FUNCTION =================
long readDistance() {
  // send the trigger pulse
  digitalWrite(3, LOW);
  delayMicroseconds(2);
  digitalWrite(3, HIGH);
  delayMicroseconds(10);
  digitalWrite(3, LOW);

  // measure echo and convert to cm
  long duration = pulseIn(2, HIGH);
  return duration / 58;
}


// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  // configure Trig and Echo pins
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);

  // configure all motor pins as OUTPUT
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  stopMotors();
  Serial.println("Distance Based Speed Control — Ready");
}


// ================= LOOP =================
void loop() {
  long distance = readDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm  |  Status: ");

  if (distance <= STOP_DISTANCE) {
    stopMotors();
    Serial.println("STOPPED");
  }
  else if (distance < SLOW_DISTANCE) {
    // scale speed proportionally between STOP_DISTANCE and SLOW_DISTANCE
    int speed = map(distance, STOP_DISTANCE, SLOW_DISTANCE, 0, FULL_SPEED);
    moveForward(speed);
    Serial.println("SLOWING");
  }
  else {
    moveForward(FULL_SPEED);
    Serial.println("FULL SPEED");
  }

  delay(100);
}