#include "detector.h"
#include "defines.h"
#include <Arduino.h>


extern bool ready1;  //sensor 1 data ready
extern bool ready2;  //sensor 2 data ready
extern bool ready3;  //sensor 3 data ready

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



/**
  Processes the grabbed data
  **/
void process(void) {

  Serial.begin(SERIAL_SPEED);


  // Light gab durations it's the courtain hole size
  gap1 = tOff1 - tOn1;
  gap2 = tOff2 - tOn2;
  gap3 = tOff3 - tOn3;



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


  /*
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
  */

  // data processed we reset the flags
  ready1 = false;
  ready2 = false;
  ready3 = false;

  switch (mode) {
    case M_CENTRAL_SHUTTER:
      centralShutter();
      break;
    case M_COURTAIN_SHUTTER:
      courtainShutter();
      break;
    case M_COURTAIN_SPEED:
      courtainSpeed();
      break;
  }
}

/* This is used for leaf shutters or focal plane shutters at slow speeds
*/
void centralShutter(void) {
  Serial.println("Process central shutter data");

  if (gap2 <= 0) {
    Serial.println("something wrong, repeat the test");
    return;
  }

  float shutterSpeed = 1.0 / gap2 * 1000000;

  if (SERIAL_PRINT) {
    Serial.print("duration: ");
    Serial.println(gap2);
    Serial.print("Speed 1/");
    Serial.println(shutterSpeed,0);
  }
}
void courtainShutter(void) {
  Serial.println("Process courtain shutter data");
}
void courtainSpeed(void) {
  Serial.println("Process courtain speed data");
}
