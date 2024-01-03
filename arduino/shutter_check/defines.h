#ifndef HEADER_DEFINES
#define HEADER_DEFINES

#define SERIAL_SPEED 9600
#define SERIAL_PRINT true

#define SENSOR1 2  //first sensor the one firstly hit
#define SENSOR2 3  //central sensor
#define SENSOR3 4  //last sensor

#define LIGHT 0 //sensor see light
#define DARK 1 //sensor in dark


#define BTN1 5  // Mode button pin
#define BTN2 6  // Page button pin

#define M_CENTRAL_SHUTTER 0   //only measure central shutter speed
#define M_COURTAIN_SHUTTER 1  //measure courtain shutter speed
#define M_COURTAIN_SPEED_H 2    //measure courtain speed for orizontal shutters
#define M_COURTAIN_SPEED_V 3    //measure courtain speed for vertical shutter

#define HORIZONTAL 0 // horizontal shutter
#define VERTICAL 1 // vertical shutter
#define CENTRAL 2 // central shutter

#define TREND_CONSTANT 0
#define TREND_SLOW_DOWN  1
#define TREND_SPEED_UP 2


#define DISTANCE 7.40  //distance in mm between sensors

#define FORMAT_24 24
#define FORMAT_36 36




#endif