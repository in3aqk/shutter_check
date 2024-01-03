#include "detector.h"
#include "defines.h"
#include <Arduino.h>


extern bool ready1;  //sensor 1 data ready
extern bool ready2;  //sensor 2 data ready
extern bool ready3;  //sensor 3 data ready

long gap1 = 0;  // t off - t on the courtain gap
long gap2 = 0;  // t off - t on the courtain gap
long gap3 = 0;  // t off - t on the courtain gap




/**
  Processes the grabbed data
  **/
void process(void) {

  Serial.begin(SERIAL_SPEED);


  // Light gab durations it's the courtain hole size
  gap1 = tOff1 - tOn1;
  gap2 = tOff2 - tOn2;
  gap3 = tOff3 - tOn3;

  if (gap1 <= 0 || gap2 <= 0 || gap3 <= 0) {
    Serial.println("something wrong, repeat the test");
    return;
  }


  // data processed we reset the flags
  ready1 = false;
  ready2 = false;
  ready3 = false;

  Serial.println(mode);

  switch (mode) {
    case M_CENTRAL_SHUTTER:
      centralShutter();
      break;
    case M_COURTAIN_SHUTTER:
      courtainShutter();
      break;
    case M_COURTAIN_SPEED_H:
      courtainSpeed(HORIZONTAL);
      break;
    case M_COURTAIN_SPEED_V:
      courtainSpeed(VERTICAL);
      break;
  }
}

/* This is used for leaf shutters or focal plane shutters at slow speeds
*/
void centralShutter(void) {
  Serial.println("Process central shutter data");

  float shutterSpeed = 1.0 / gap2 * 1000000;

  if (SERIAL_PRINT) {    
    Serial.print("Speed 1/");
    Serial.println(shutterSpeed, 0);
  }
}
void courtainShutter(void) {
  Serial.println("Process courtain shutter data");
  float gapM = (gap1 + gap2 + gap3) / 3;
  float shutterSpeed = 1.0 / gapM * 1000000;

  if (SERIAL_PRINT) {
    Serial.println(gapM);
    Serial.print("Speed 1/");
    Serial.println(shutterSpeed, 0);
  }
}
void courtainSpeed(int direction) {

  // travel time of the first courtain over the sensors
  long firstTravel12;  // time distance led 1 to 2
  long firstTravel23;  // time distance led 2 to 3
  long firstTravel13;  // time distance led 1 to 3
  float firstTravelM;  // total travel time over 35 mm
  float firstTravelT;  // total travel time over 35 mm
  float firstSpeed;    // speed over led 1 to 3 in mm/s

  // travel time of the second courtain over the sensors
  long secondTravel12;  // time distance led 1 to 2
  long secondTravel23;  // time distance led 2 to 3
  long secondTravel13;  // time distance led 1 to 3
  float secondTravelM;  // total travel time over 35 mm
  float secondTravelT;  // total travel time over 35 mm
  float secondSpeed;    // speed over led 1 to 3 in mm/s

  int first_speed_trend;
  int second_speed_trend;

  firstTravel12 = tOn2 - tOn1;
  firstTravel23 = tOn3 - tOn2;
  firstTravel13 = tOn3 - tOn1;

  secondTravel12 = tOff2 - tOff1;
  secondTravel23 = tOff3 - tOff2;
  secondTravel13 = tOff3 - tOff1;



  firstTravelM = mediumTravel(firstTravel12, firstTravel23, firstTravel13);      //Mean gap 4 because of 13
  secondTravelM = mediumTravel(secondTravel12, secondTravel23, secondTravel13);  //Mean gap 4 because of 13

  first_speed_trend = trend(firstTravel12, firstTravel23);
  second_speed_trend = trend(secondTravel12, secondTravel23);

  if (direction == HORIZONTAL) {
    firstTravelT = totalTravel(firstTravelM, FORMAT_36);
    firstSpeed = speed(firstTravelT, FORMAT_36);
    secondTravelT = totalTravel(secondTravelM, FORMAT_36);
    secondSpeed = speed(secondTravelT, FORMAT_36);
  } else {
    firstTravelT = totalTravel(firstTravelM, FORMAT_24);
    firstSpeed = speed(firstTravelT, FORMAT_24);
    secondTravelT = totalTravel(secondTravelM, FORMAT_24);
    secondSpeed = speed(secondTravelT, FORMAT_24);
  }

  if (SERIAL_PRINT) {

    Serial.println("Process courtain speed data");

    Serial.println("---TIME---");
    Serial.print("first courtain travel time: ");
    Serial.println(firstTravelT);

    Serial.print("second courtain travel time: ");
    Serial.println(secondTravelT);


    Serial.println("---SPEED---");
    Serial.print("first courtain speed: ");
    Serial.print(firstSpeed, 2);
    Serial.println(" mm/s");
    Serial.print("first courtain trend: ");
    Serial.println(first_speed_trend);

    Serial.print("second courtain speed: ");
    Serial.print(secondSpeed, 2);
    Serial.println(" mm/s");
    Serial.print("second courtain trend: ");
    Serial.println(second_speed_trend);

    Serial.print("speed diff: ");
    Serial.println(firstTravelT-secondTravelT);

    if (first_speed_trend == second_speed_trend) {
      Serial.println("Speed trend in same direction!");
    }
  }
}


float totalTravel(float travel, int frameSize) {
  return (travel * frameSize / (2 * DISTANCE));
}

float mediumTravel(long travel12, long travel23, long travel13) {
  return (travel12 + travel23 + travel13) / 4;  //Mean gap 4 becaouse of 13
}

float speed(float time, int format) {
  return format / time * 1000000;
}

int trend(long travel12, long travel23) {
  if (travel23 > travel12) {
    return TREND_SLOW_DOWN;
  } else if (travel23 < travel12) {
    return TREND_SPEED_UP;
  } else {
    return TREND_CONSTANT;
  }
}
