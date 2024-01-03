#ifndef HEADER_DETECTOR
#define HEADER_DETECTOR

extern int mode;

// all times are in microseconds speed in mm/s
extern unsigned long tOn1;
extern unsigned long tOn2;
extern unsigned long tOn3;

extern unsigned long tOff1;
extern unsigned long tOff2;
extern unsigned long tOff3;


extern bool ready1;  //sensor 1 data ready
extern bool ready2;  //sensor 2 data ready
extern bool ready3;  //sensor 3 data ready


void process(void);
void centralShutter(void);
void courtainShutter(void);
void courtainSpeed(int);

float totalTravel(float, int);
float mediumTravel(long, long, long);
float speed(float, int);
int trend(long,long);

#endif