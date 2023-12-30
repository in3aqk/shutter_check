#include "defines.h"
#include "OneButton.h"


OneButton btnMode = OneButton(BTN1, false, true);
OneButton btnPage = OneButton(BTN2, false, true);

int sensor1;
int sensor2;
int sensor3;

bool mode = MODE_SHUTTER;

bool on1 = false;  //sensor 1 on flag
bool on2 = false;  //sensor 2 on flag
bool on3 = false;  //sensor 3 on flag

bool ready1 = false;  //sensor 1 data ready
bool ready2 = false;  //sensor 2 data ready
bool ready3 = false;  //sensor 3 data ready

// all times are in microseconds speed in mm/s
unsigned long tOn1 = 0;
unsigned long tOn2 = 0;
unsigned long tOn3 = 0;

unsigned long tOff1 = 0;
unsigned long tOff2 = 0;
unsigned long tOff3 = 0;

long gap1 = 0;  // t off - t on the courtain gap
long gap2 = 0;  // t off - t on the courtain gap
long gap3 = 0;  // t off - t on the courtain gap

long travel12 = 0;  // time distance led 1 to 2
long travel23 = 0;  // time distance led 2 to 3
long travel13 = 0;  // time distance led 1 to 3
long travelt = 0;   // total travel time over 35 mm

float speed12 = 0;  // speed over led 1 to 2 in mm/s
float speed23 = 0;  // speed over led 2 to 3 in mm/s
float speedt = 0;   // speed over led 1 to 3 in mm/s

// Setup runs once, at start
// Input and Output pins are set
void setup() {
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);

  btnMode.attachClick(modeClick);
  btnPage.attachClick(pageClick);

  Serial.begin(SERIAL_SPEED);
  Serial.println("READY!");
  Serial.println("mode: shutter");
}


void loop() {

  btnMode.tick();
  btnPage.tick();

  get();
  if (ready1 && ready2 && ready3) {  //we have all datas and we can process!
    process();
  }
}

/**
  Grabs data
  **/

void get(void) {

  // Get the Sensor status
  sensor1 = digitalRead(SENSOR1);
  sensor2 = digitalRead(SENSOR2);
  sensor3 = digitalRead(SENSOR3);

  if (sensor1 == 0 && !on1) {
    on1 = true;
    tOn1 = micros();
    //Serial.println("1 on");
  }

  if (sensor1 == 1 && on1) {
    on1 = false;
    tOff1 = micros();
    ready1 = true;
  }

  if (sensor2 == 0 && !on2) {
    on2 = true;
    tOn2 = micros();
    //Serial.println("1 on");
  }

  if (sensor2 == 1 && on2) {
    on2 = false;
    tOff2 = micros();
    ready2 = true;
  }

  if (sensor3 == 0 && !on3) {
    on3 = true;
    tOn3 = micros();
    //Serial.println("1 on");
  }

  if (sensor3 == 1 && on3) {
    on3 = false;
    tOff3 = micros();
    ready3 = true;
  }
}

/**
  Processes the grabbed data
  **/
void process(void) {

  // Light gab durations it's the courtain hole size
  gap1 = tOff1 - tOn1;
  gap2 = tOff2 - tOn2;
  gap3 = tOff3 - tOn3;
  float gapM = (gap1 + gap2 + gap3) / 3;  //Mean gap

  // Shutter speed TODO non sure
  float shutterSpeed = 1 / gapM * 1000000;


  // travel time of the first courtain over the sensors
  travel12 = tOn2 - tOn1;
  travel23 = tOn3 - tOn2;
  travel13 = tOn3 - tOn1;
  float travelM = (travel12 + travel23 + travel13) / 4;  //Mean gap 4 beacouse of 13
  float travelt = (travelM * 36 / 2 / DISTANCE);

  speed12 = DISTANCE / travel12 * 1000000;
  speed23 = DISTANCE / travel23 * 1000000;
  speedt = DISTANCE * 2 / travelt * 1000000;

  float gapSizeA = gap1 * speed12 / 1000000;
  float gapSizeB = gap2 * speed12 / 1000000;
  float gapSizeC = gap3 * speed23 / 1000000;
  float gapSizeT = gap2 * speedt / 1000000;



  if (SERIAL_PRINT) {

    Serial.println("---TIME---");
    Serial.print("gap 1: ");
    Serial.println(gap1);
    Serial.print("gap 2: ");
    Serial.println(gap2);
    Serial.print("gap 3: ");
    Serial.println(gap3);
    Serial.print("gap M: ");
    Serial.println(gapM);
    Serial.print("Speed 1/");
    Serial.println(shutterSpeed, 0);

    Serial.println("---TRAVEL---");
    Serial.print("travel12: ");
    Serial.println(travel12);
    Serial.print("travel23: ");
    Serial.println(travel12);
    Serial.print("travel13: ");
    Serial.println(travel13);
    Serial.print("travelM: ");
    Serial.println(travelM);
    Serial.print("travelt: ");
    Serial.println(travelt);

    Serial.println("---SPEED---");
    Serial.print("speed12: ");
    Serial.print(speed12, 2);
    Serial.println(" mm/s");
    Serial.print("speed23: ");
    Serial.print(speed23, 2);
    Serial.println(" mm/s");
    Serial.print("speedt: ");
    Serial.print(speedt, 2);
    Serial.println(" mm/s");

    Serial.println("---GAP_SIZE---");
    Serial.print("A: ");
    Serial.println(gapSizeA);
    Serial.print("B: ");
    Serial.print(gapSizeB);
    Serial.println(" mm");
    Serial.print("C: ");
    Serial.print(gapSizeC);
    Serial.println(" mm");
    Serial.print("T: ");
    Serial.print(gapSizeT);
    Serial.println(" mm");
  }

  // data processed we reset the flags
  ready1 = false;
  ready2 = false;
  ready3 = false;
}



static void modeClick() {
  if (mode == MODE_SHUTTER) {
    mode = MODE_COURTAIN;
    Serial.println("mode: courtain");
  } else {
    mode = MODE_SHUTTER;
    Serial.println("mode: shutter");
  }
}

static void pageClick() {
  Serial.println("page");
}
