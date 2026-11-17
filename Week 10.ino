#include <IRArray.h>
#include <Wire.h>
#include <Adafruit_PCF8574.h>
#include <Adafruit_NeoPixel.h>

// ========== MOTOR PINS ==========
#define AIN1 A1
#define AIN2 A0
#define PWMA 6

#define BIN1 A2
#define BIN2 A3
#define PWMB 5

// ========== SENSOR PINS ==========
#define TRIG_PIN 3
#define ECHO_PIN 2

#define LED_PIN 7
#define NUM_LEDS 4
#define BUZZER_PIN 5

// ========== SETTINGS ==========
#define THRESHOLD 959
#define BASE_SPEED 40
#define MAX_SPEED 65
#define TURN_SPEED 40
#define OBSTACLE_DISTANCE 12
#define START_DELAY 3000

float Kp = 16.0;
float Kd = 6.0;

// ========== OBJECTS ==========
IRArray ir;
unsigned int sensorValues[5];

Adafruit_PCF8574 pcf;
Adafruit_NeoPixel leds(
  NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800
);

int weights[5] = {-2, -1, 0, 1, 2};

float previousError = 0;
float lastKnownError = 0;

bool finished = false;
bool turning = false;
bool obstacleDetected = false;
bool junctionLocked = false;

int turnDirection = 0;
unsigned long turnStart = 0;

// ========== MOTOR FUNCTIONS ==========
void moveForward(int leftSpeed, int rightSpeed) {

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  analogWrite(PWMA, constrain(leftSpeed, 0, 255));
  analogWrite(PWMB, constrain(rightSpeed, 0, 255));
}

void turnLeft() {
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);

  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);

  analogWrite(PWMA,TURN_SPEED);
  analogWrite(PWMB,TURN_SPEED);
}

void turnRight() {
  
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);

  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);

  analogWrite(PWMA,TURN_SPEED);
  analogWrite(PWMB,TURN_SPEED);
}

void stopMotors() {

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);

  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

// ========== ULTRASONIC SENSOR ==========
float getDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration =
    pulseIn(ECHO_PIN, HIGH, 12000UL);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2.0;
}

// ========== IR SENSOR FUNCTIONS ==========
void readSensors() {
  ir.read(sensorValues);
  for(int i=0;i<5;i++){
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();
}

bool isWhite(int i) {
  return sensorValues[i] > THRESHOLD;
}

int countWhite() {

  int count = 0;

  for (int i = 0; i < 5; i++) {
    if (isWhite(i)) {
      count++;
    }
  }

  return count;
}

// ========== RGB LED FUNCTION ==========
void updateLEDs() {

  int sensorIndex[4] = {0, 1, 3, 4};

  for (int i = 0; i < 4; i++) {

    if (isWhite(sensorIndex[i])) {
      leds.setPixelColor(
        i, leds.Color(80, 0, 0)
      );
    }
    else {
      leds.setPixelColor(
        i, leds.Color(0, 80, 0)
      );
    }
  }

  leds.show();
}

// ========== ERROR CALCULATION ==========
float calculateError() {

  float sum = 0;
  int count = 0;

  for (int i = 0; i < 5; i++) {

    if (isWhite(i)) {
      sum += weights[i];
      count++;
    }
  }

  if (count == 0) {
    if(previousError>0)
       return 2;
    if(previousError<0)
       return -2;
    return 0;
  }

  float error = sum / count;

  lastKnownError = error;

  return error;
}

// ========== PD LINE FOLLOWING ==========
void followLine() {

  float error = calculateError();

  float derivative = error - previousError;

  float correction =
    Kp * error + Kd * derivative;

correction= constrain(correction,-45,45);

  int leftSpeed = constrain(
    BASE_SPEED + correction,
    0, MAX_SPEED
  );

  int rightSpeed = constrain(
    BASE_SPEED - correction,
    0, MAX_SPEED
  );
  
  moveForward(leftSpeed, rightSpeed);

  previousError = error;
}

// ========== JUNCTION DETECTION ==========
bool isJunction() {

 
  return countWhite()>=3;
}

// ========== BRANCH SELECTION ==========
int chooseBranch() {

  int leftCount = 0;
  int rightCount = 0;

  if (isWhite(0)) leftCount++;
  if (isWhite(1)) leftCount++;

  if (isWhite(3)) rightCount++;
  if (isWhite(4)) rightCount++;

  if (leftCount > rightCount) {
    return -1;
  }

  if (rightCount > leftCount) {
    return 1;
  }

  return 0;
}

// ========== START TURN ==========
void startTurn(int direction) {

  turning = true;
  turnDirection = direction;
  turnStart = millis();

  previousError = 0;
}

// ========== HANDLE TURN ==========
void handleTurn() {

   if (turnDirection == 1) {
    turnRight();
    }
   else{
    turnLeft();
  }
  

  if (millis() - turnStart >=420) {
    turning=false;
    obstacleDetected=false;
    junctionLocked=true;
    previousError=0;
    lastKnownError=0;
    

  }

  // Centre sensor finds the new line
  if ( isWhite(2)&& isWhite(3)) {

    turning = false;
    obstacleDetected = false;
    junctionLocked = true;

    previousError = 0;
    lastKnownError= calculateError();

  }

  // Safety timeout
  if (millis() - turnStart > 1500) {
    turning = false;
    obstacleDetected = false;
    previousError = 0;
    stopMotors();
  }
}

// ========== BUZZER ==========
void beepThreeTimes() {

  for (int i = 0; i < 3; i++) {

    pcf.digitalWrite(BUZZER_PIN, LOW);
    delay(200);

    pcf.digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
  }
}

// ========== FINISH ==========
void finishRun() {

  stopMotors();
  finished = true;

  for (int i = 0; i < 4; i++) {
    leds.setPixelColor(
      i, leds.Color(80, 0, 0)
    );
  }

  leds.show();

  beepThreeTimes();
}

// ========== SETUP ==========
void setup() {

  Serial.begin(115200);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  ir.begin();

  leds.begin();
  leds.clear();
  leds.show();

  Wire.begin();

  if (!pcf.begin(0x20, &Wire)) {
    stopMotors();
    while (true) {
      delay(100);
    }
  }

  pcf.pinMode(BUZZER_PIN, OUTPUT);
  pcf.digitalWrite(BUZZER_PIN, HIGH);

  stopMotors();

  delay(START_DELAY);
}

// ========== MAIN LOOP ==========
void loop() {

  if (finished) {
    stopMotors();
    return;
  }

  // Read IR sensors
  readSensors();
  updateLEDs();

  // END: all 5 sensors detect white
  if (!turning && countWhite() == 5) {

    stopMotors();
    delay(80);

    readSensors();
    updateLEDs();

    if (countWhite() == 5) {
      finishRun();
      return;
    }
  }

  // Continue turning
  if (turning) {
    handleTurn();
    return;
  }

  // Reset junction lock after leaving it
  if (!isJunction()) {
    junctionLocked = false;
  }

  // Check obstacle
  float distance = getDistance();

  if (distance > 0 &&
      distance < OBSTACLE_DISTANCE) {

    obstacleDetected = true;
  }
  else if (distance >= OBSTACLE_DISTANCE) {
    obstacleDetected = false;
  }

  // Obstacle and junction: take branch
  if (obstacleDetected  && !turning) {

    
      startTurn(1);
      return;
    }

    

  // Stop if too close to obstacle
  if (obstacleDetected &&
      distance > 0 &&
      distance < 8) {

    stopMotors();
    return;
  }

  // Normal white-line following
  followLine();
}