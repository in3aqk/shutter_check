
const int sensorPin1 = 2;
const int sensorPin2 = 3;
const int sensorPin3 = 4;

const float d1 = 7.4;
const float d2 = 7.4;
const float d3 = 7.4;



int sensor1;
int sensor2;
int sensor3;
bool on1 = false;
bool on2 = false;
bool on3 = false;

unsigned long tOn1 = 0;
unsigned long tOn2 = 0;
unsigned long tOn3 = 0;

unsigned long tOff1 = 0;
unsigned long tOff2 = 0;
unsigned long tOff3 = 0;

unsigned long duration1 = 0;
unsigned long duration2 = 0;
unsigned long duration3 = 0;

float gap1 = 0;
float gap2 = 0;
float gap3 = 0;

float speed1 = 0;
float speed2 = 0;

// Setup runs once, at start
// Input and Output pins are set
void setup() {
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  Serial.begin(9600);
  Serial.println("READY");
}


void loop() {

  check();
}


void check(void) {

  // Get the Sensor status
  sensor1 = digitalRead(sensorPin1);
  sensor2 = digitalRead(sensorPin2);
  sensor3 = digitalRead(sensorPin3);

  if (sensor1 == 0 && !on1) {
    on1 = true;
    tOn1 = millis();
    //Serial.println("1 on");
  }

  if (sensor1 == 1 && on1) {
    on1 = false;
    tOff1 = millis();
    //Serial.println("1 of");
    duration1 = tOff1 - tOn1;    
    Serial.print("duration 1: ");
    Serial.println(duration1);
    
  }

  if (sensor2 == 0 && !on2) {
    on2 = true;
    tOn2 = millis();
    //Serial.println("1 on");
  }

  if (sensor2 == 1 && on2) {
    on2 = false;
    tOff2 = millis();    
    duration2 = tOff2 - tOn2;
    unsigned long tGap1 = tOn2 - tOn1;
    Serial.print("duration 2: ");
    Serial.println(duration2);
    Serial.print("gap 1: ");
    Serial.println(tGap1);
  }

  if (sensor3 == 0 && !on3) {
    on3 = true;
    tOn3 = millis();
    //Serial.println("1 on");
  }

  if (sensor3 == 1 && on3) {
    on3 = false;
    tOff3 = millis();    
    duration3 = tOff3 - tOn3;
    unsigned long tGap2 = tOn3 - tOn2;
    Serial.print("duration 3: ");
    Serial.println(duration3);
    Serial.print("gap 2: ");
    Serial.println(tGap2);
  }
}
