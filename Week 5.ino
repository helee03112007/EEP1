#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// ============================================================
//                 ALPHABOT2-AR WEEK 5 - BUMPER CARS
// ============================================================

// --- MOTOR PINS ---
const int PWMA = 6;
const int AIN2 = A0;
const int AIN1 = A1;

const int PWMB = 5;
const int BIN1 = A2;
const int BIN2 = A3;

// --- ULTRASONIC SENSOR ---
const int TRIG_PIN = 3;
const int ECHO_PIN = 2;

// --- RGB LED (WS2812B) ---
const int RGB_PIN = 7;
const int NUM_LEDS = 4;

Adafruit_NeoPixel leds(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

// --- BUZZER (I2C PCF8574) ---
const byte PCF8574_ADDRESS = 0x20;
const byte BUZZER_BIT = 5;

// --- SPEED SETTINGS ---
const int LEFT_SPEED  = 62;
const int RIGHT_SPEED = 60;
const int SLOW_SPEED  = 40;  

// Lowered slightly to provide precise control during turns
const int TURN_SPEED  = 45;  

// --- TURN SETTINGS ---
// Reduce this value by 10-20 ms if it still over-rotates (e.g., 290)
const int TURN_90_MS = 260; 
const int MAX_TURNS  = 3;

// --- DISTANCE SETTINGS ---
const float WALL_DISTANCE     = 10.0;
const float APPROACH_DISTANCE = 30.0;
const float CLEAR_DISTANCE    = 15.0;

// --- GLOBAL VARIABLES ---
int wallCount = 0;

void setup() {
  Serial.begin(9600);

  // Motor Pins Setup
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Ultrasonic Pins Setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  // LED Setup
  leds.begin();
  leds.setBrightness(80);
  setLEDColor(0, 0, 0);

  // I2C & Buzzer Setup
  Wire.begin();
  buzzerOff();

  stopMotors();
  delay(1000);

  Serial.println("==============================");
  Serial.println(" ALPHABOT2-AR WEEK 5: BUMPER CARS");
  Serial.println(" Robot Ready!");
  Serial.println("==============================");
  delay(4000);
}

void loop() {
  float distance = readDistance();

  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.print(" cm | Walls = ");
  Serial.println(wallCount);

  // ==========================================================
  // CASE 1: DISTANCE > 30 CM (GREEN LED - DRIVE FORWARD)
  // ==========================================================
  if (distance > APPROACH_DISTANCE) {
    setLEDColor(0, 255, 0); // Green
    driveForward(LEFT_SPEED, RIGHT_SPEED);
  }

  // ==========================================================
  // CASE 2: 10 CM < DISTANCE <= 30 CM (YELLOW LED - WALL APPROACHING)
  // ==========================================================
  else if (distance > WALL_DISTANCE) {
    setLEDColor(255, 255, 0); // Yellow

    if (distance <= CLEAR_DISTANCE) {
      driveForward(SLOW_SPEED, SLOW_SPEED);
      Serial.println("Approaching wall - SLOW SPEED");
    } else {
      driveForward(LEFT_SPEED, RIGHT_SPEED);
    }
  }

  // ==========================================================
  // CASE 3: DISTANCE <= 10 CM (RED LED - WALL REACHED)
  // ==========================================================
  else {
    Serial.println("\n***********************\n  WALL DETECTED\n***********************");
    
    setLEDColor(255, 0, 0); // Red
    
    // 1. ACTIVE BRAKE: Fully halt robot momentum before turning
    stopMotors();
    delay(200); 

    wallCount++;
    Serial.print("Wall count = ");
    Serial.println(wallCount);

    // --- WALLS 1, 2, AND 3: TURN 90 DEGREES RIGHT ---
    if (wallCount <= MAX_TURNS) {
      Serial.println("Executing 90-degree Right Turn...");

      turnRight(TURN_SPEED);
      delay(TURN_90_MS);

      stopMotors();
      
      // 2. TURN COOLDOWN: Prevent sensor from double-detecting the same wall
      delay(500); 

      Serial.println("Turn completed. Resuming run...\n");
    } 
    // --- WALL 4: STOP & BEEP 3 SECONDS (END OF RUN) ---
    else {
      Serial.println("\n==============================\n 4TH WALL - RUN COMPLETE\n==============================");

      stopMotors();
      setLEDColor(255, 0, 0);

      // Sound Buzzer for 3 seconds
      Serial.println("BUZZER ON");
      buzzerOn();
      delay(3000);
      buzzerOff();
      Serial.println("BUZZER OFF");

      // Permanently end execution
      while (true) {
        stopMotors();
        delay(500);
      }
    }
  }

  delay(50);
}

// ============================================================
//                  HARDWARE HELPER FUNCTIONS
// ============================================================

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 400.0;

  return (duration * 0.0343) / 2.0;
}

void driveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  analogWrite(PWMA, leftSpeed);
  analogWrite(PWMB, rightSpeed);
}

void turnRight(int speed) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
}

void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}

void setLEDColor(int red, int green, int blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, leds.Color(red, green, blue));
  }
  leds.show();
}

void pcfWrite(byte value) {
  Wire.beginTransmission(PCF8574_ADDRESS);
  Wire.write(value);
  Wire.endTransmission();
}

void buzzerOn() {
  byte value = 0xFF;
  value &= ~(1 << BUZZER_BIT);
  pcfWrite(value);
}

void buzzerOff() {
  byte value = 0xFF;
  value |= (1 << BUZZER_BIT);
  pcfWrite(value);
}