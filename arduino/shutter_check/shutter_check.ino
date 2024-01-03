#include "defines.h"
#include "detector.h"
#include "OneButton.h"


OneButton btnMode = OneButton(BTN1, false, true);
OneButton btnPage = OneButton(BTN2, false, true);

int sensor1;
int sensor2;
int sensor3;

int mode = M_COURTAIN_SPEED_H;

bool on1 = false;  //sensor 1 on flag
bool on2 = false;  //sensor 2 on flag
bool on3 = false;  //sensor 3 on flag

bool ready1 = false;  //sensor 1 data ready
bool ready2 = false;  //sensor 2 data ready
bool ready3 = false;  //sensor 3 data ready

unsigned long last_time;

// all times are in microseconds speed in mm/s
unsigned long tOn1 = 0;
unsigned long tOn2 = 0;
unsigned long tOn3 = 0;

unsigned long tOff1 = 0;
unsigned long tOff2 = 0;
unsigned long tOff3 = 0;



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
  //Serial.println("mode: shutter");
}


void loop() {

  btnMode.tick();
  btnPage.tick();

  if (micros() > last_time) {  //avoid sampling if micros overflow
    get();
    if (ready1 && ready2 && ready3) {  //we have all datas and we can process!
      process();
    }
  }
  last_time = micros();
}

/**
  Grabs data
  **/

void get(void) {

  // Get the Sensor status
  sensor1 = digitalRead(SENSOR1);
  sensor2 = digitalRead(SENSOR2);
  sensor3 = digitalRead(SENSOR3);

  if (sensor1 == LIGHT && !on1) {
    on1 = true;
    tOn1 = micros();
  }

  if (sensor1 == DARK && on1) {
    on1 = false;
    tOff1 = micros();
    ready1 = true;
  }

  if (sensor2 == LIGHT && !on2) {
    on2 = true;
    tOn2 = micros();
  }

  if (sensor2 == DARK && on2) {
    on2 = false;
    tOff2 = micros();
    ready2 = true;
  }

  if (sensor3 == LIGHT && !on3) {
    on3 = true;
    tOn3 = micros();
  }

  if (sensor3 == DARK && on3) {
    on3 = false;
    tOff3 = micros();
    ready3 = true;
  }
}


static void modeClick() {  
  if (mode < M_COURTAIN_SPEED_V) {
    mode += 1;
  } else {
    mode = M_CENTRAL_SHUTTER;
  }
  Serial.print("Mode: ");
  switch (mode) {
    case M_CENTRAL_SHUTTER:
      Serial.println("Central shutter");      
      break;
    case M_COURTAIN_SHUTTER:
      Serial.println("Courtain shutter");
      break;
    case M_COURTAIN_SPEED_H:
      Serial.println("Courtain speed horizzonatal");
      Serial.println("SET SPEED TO MODE B OR FLASH SYNC TIME OR LOWER!!");
      break;
    case M_COURTAIN_SPEED_V:
      Serial.println("Courtain speed vertical");
      Serial.println("SET SPEED TO MODE B OR FLASH SYNC TIME OR LOWER!!");
      break;
  }
}

static void pageClick() {
  Serial.println("page");
}
