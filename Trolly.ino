// --- Pin Definitions ---
const int motorPin    = 10;  // Signal to run motor (HIGH = run, LOW = stop)
const int motorDirPin = 11;  // Direction control (HIGH = UP, LOW = DOWN)

const int remoteA = 25;      // RF input from relay A (momentary) - UP button
const int remoteB = 12;      // RF input from relay B (momentary) - DOWN button

// Limit sensors (adjust pin numbers as needed)
const int sensorTopPin = 2;     // Top limit sensor
const int sensorBtmPin = 3;     // Bottom limit sensor

// --- State Variables ---
bool sensorTop = false;
bool sensorBtm = false;
bool motorRunning = false;
bool motorDirection = false;    // false = DOWN, true = UP

// Debouncing variables
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  // --- Motor control pins ---
  pinMode(motorPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  digitalWrite(motorPin, LOW);        // Motor off initially
  digitalWrite(motorDirPin, LOW);     // Default direction (DOWN)

  // --- RF relay inputs ---
  pinMode(remoteA, INPUT_PULLUP);     // UP button
  pinMode(remoteB, INPUT_PULLUP);     // DOWN button

  // --- Limit sensor inputs ---
  pinMode(sensorTopPin, INPUT_PULLUP);    // Adjust INPUT type based on your sensor
  pinMode(sensorBtmPin, INPUT_PULLUP);    // Adjust INPUT type based on your sensor

  // --- Debugging ---
  Serial.begin(115200);
  Serial.println("Trolley controller initialized.");
  Serial.println("RemoteA (Pin 25) = UP, RemoteB (Pin 12) = DOWN");
}

void loop() {
  // Read limit sensors
  readSensors();
  
  // Read remote inputs with debouncing
  readRemoteInputs();
  
  // Main control logic
  controlMotor();
  
  delay(10);  // Small delay for stability
}

void readSensors() {
  // Read sensor states - adjust logic based on your sensor type
  // Assuming sensors are active LOW (triggered when LOW)
  sensorTop = digitalRead(sensorTopPin) == LOW;
  sensorBtm = digitalRead(sensorBtmPin) == LOW;
  
  // Normal operation - stop when reaching limits
  if (motorRunning) {
    if (sensorTop && motorDirection == true) {
      Serial.println("Reached top limit - Stopping");
      stopMotor();
    }
    else if (sensorBtm && motorDirection == false) {
      Serial.println("Reached bottom limit - Stopping");
      stopMotor();
    }
  }
}

void readRemoteInputs() {
  static bool lastUpState = false;
  static bool lastDownState = false;
  
  bool upPressed = digitalRead(remoteA) == LOW;
  bool downPressed = digitalRead(remoteB) == LOW;
  
  // Debounce logic - only process button presses on rising edge
  if (millis() - lastDebounceTime > debounceDelay) {
    // UP button pressed (rising edge detection)
    if (upPressed && !lastUpState) {
      if (motorRunning) {
        // If motor is running, stop it regardless of direction
        Serial.println("UP pressed while motor running - Stopping");
        stopMotor();
      } else {
        // If motor is stopped, start moving UP
        handleUpRequest();
      }
      lastDebounceTime = millis();
    }
    
    // DOWN button pressed (rising edge detection)
    if (downPressed && !lastDownState) {
      if (motorRunning) {
        // If motor is running, stop it regardless of direction
        Serial.println("DOWN pressed while motor running - Stopping");
        stopMotor();
      } else {
        // If motor is stopped, start moving DOWN
        handleDownRequest();
      }
      lastDebounceTime = millis();
    }
    
    lastUpState = upPressed;
    lastDownState = downPressed;
  }
}

void handleUpRequest() {
  Serial.println("UP button pressed");
  
  // Safety check - don't move up if already at top
  if (sensorTop) {
    Serial.println("At top limit - UP request ignored");
    return;
  }
  
  // Set direction and start motor
  motorDirection = true;  // UP
  digitalWrite(motorDirPin, HIGH);
  digitalWrite(motorPin, HIGH);
  motorRunning = true;
  
  Serial.println("Motor moving UP");
}

void handleDownRequest() {
  Serial.println("DOWN button pressed");
  
  // Safety check - don't move down if already at bottom
  if (sensorBtm) {
    Serial.println("At bottom limit - DOWN request ignored");
    return;
  }
  
  // Set direction and start motor
  motorDirection = false;  // DOWN
  digitalWrite(motorDirPin, LOW);
  digitalWrite(motorPin, HIGH);
  motorRunning = true;
  
  Serial.println("Motor moving DOWN");
}

void controlMotor() {
  // This function is now handled by readSensors()
  // Keeping it for potential future use or additional safety checks
}

void stopMotor() {
  digitalWrite(motorPin, LOW);
  motorRunning = false;
  Serial.println("Motor STOPPED");
}

// Optional: Function to get current status
void printStatus() {
  Serial.print("Motor: ");
  Serial.print(motorRunning ? "ON" : "OFF");
  Serial.print(", Direction: ");
  Serial.print(motorDirection ? "UP" : "DOWN");
  Serial.print(", Top sensor: ");
  Serial.print(sensorTop ? "TRIGGERED" : "CLEAR");
  Serial.print(", Bottom sensor: ");
  Serial.println(sensorBtm ? "TRIGGERED" : "CLEAR");
}

// Optional: Call this in loop() every few seconds for debugging
void debugStatus() {
  static unsigned long lastStatusTime = 0;
  if (millis() - lastStatusTime > 2000) {  // Print status every 2 seconds
    printStatus();
    lastStatusTime = millis();
  }
}