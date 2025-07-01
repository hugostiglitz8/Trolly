// --- Pin Definitions ---
const int motorPin    = 10;  // Signal to run motor (HIGH = run, LOW = stop)
const int motorDirPin = 11;  // Direction control (HIGH = UP, LOW = DOWN)

const int remoteA = 25;      // RF input from relay A (momentary)
const int remoteB = 12;      // RF input from relay B (momentary)


bool sensorTop;
bool sensorBtm;

bool direction; //LOW = Down | HIGH== UP
bool isOn = false;  

void setup() {
  // --- Motor control pins ---
  pinMode(motorPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  digitalWrite(motorPin, LOW);        // Motor off initially
  digitalWrite(motorDirPin, LOW);     // Default direction (DOWN)

  // --- RF relay inputs ---
  pinMode(remoteA, INPUT_PULLUP);
  pinMode(remoteB, INPUT_PULLUP);

  // --- Optional Debugging ---
  Serial.begin(115200);
  Serial.println("Trolley controller initialized.");
}

void loop() {
  // Read remote inputs (active LOW)
  bool upRequest   = digitalRead(remoteA) == LOW;
  bool downRequest = digitalRead(remoteB) == LOW;

  if (upRequest && !downRequest) {
    Serial.println("UP requested");
    digitalWrite(motorDirPin, HIGH);   // Set direction UP
    digitalWrite(motorPin, HIGH);      // Turn motor ON
  }
  else if (downRequest && !upRequest) {
    Serial.println("DOWN requested");
    digitalWrite(motorDirPin, LOW);    // Set direction DOWN
    digitalWrite(motorPin, HIGH);      // Turn motor ON
  }
  else {
    // No button pressed or both pressed — STOP motor
    digitalWrite(motorPin, LOW);
  }

  delay(50);  // Polling debounce
}

void remoteScan(){
  bool buttonA   = digitalRead(remoteA) == LOW;
  bool buttonB = digitalRead(remoteB) == LOW;


}