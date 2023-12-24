
const int sensorPin1 = 2;
const int sensorPin2 = 3;
const int sensorPin3 = 4;
const int TIMEOUT = 3000;  // milliseconds

int status1;
int status2;
int status3;


// Setup runs once, at start
// Input and Output pins are set
void setup() {
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  Serial.begin(9600);
  Serial.print("INIT");
}


void loop() {

  check();
}


void check(void) {

  // Get the Sensor status
  status1 = digitalRead(sensorPin1);
  status2 = digitalRead(sensorPin2);
  status3 = digitalRead(sensorPin3);

  if (status1 == 0) {
    Serial.println("trigger1");
  }

  if (status2 == 0) {
    Serial.println("trigger2");
  }

  if (status3 == 0) {
    Serial.println("trigger3");
  }
}
