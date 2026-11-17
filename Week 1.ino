 /*
  Week 1 - Graded Activity: AlphaBot2 Startup Animation

  Learning objectives:
  - Combine digitalWrite(), delay(), and Serial.println() in one sequence
  - Use a variable that updates once per loop, separate from the LED timing itself

  Instructions:
  - Print a startup message once in setup()
  - In loop(), blink the LED fast twice, then once slowly
  - Increase the cycle counter once per loop and print it
  - Print "System Ready" at the end of each cycle
  - Fill the blanks " _______ " -> " with something as instructed "
*/

// ================= PIN DEFINITION =================
#define LED_PIN 13    // onboard LED

// ================= VARIABLE =================
int cycleCount = 0;   // counts how many times the loop (one full animation) has run

// ================= SETUP =================
void setup() {
  // TODO: Start Serial communication at 9600 baud
  Serial.begin(9600);

  // TODO: Set LED_PIN as OUTPUT
  pinMode(13, OUTPUT);

  // TODO: Print a startup message
  Serial.println("Robot Robot");
}
int counter=0;
// ================= LOOP =================
void loop() {
  
  // TODO: Blink the LED fast, twice
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW); 
  delay(500);
  digitalWrite(13, HIGH); 
  delay(500);
  digitalWrite(13, LOW); 
  delay(500);

  // TODO: Blink the LED once, slowly
  digitalWrite(13, HIGH); 
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);

  // TODO: Increase the cycle counter by 1
  counter = counter + 1;

  // TODO: Print the cycle counter
  Serial.print("Cycles Completed:");
  Serial.println(counter);
}