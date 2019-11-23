//magnetLectorDriver.h
#ifndef _MAGNETLECTOR_H_
#define _MAGNETLECTOR_H_
#define NO_TRACK_DATAID -1
#define TRACK1_DATAID 1
#define TRACK2_DATAID 2
#define TRACK3_DATAID 3

#define MAX_ITEMS_LECTOR_QUEUE 5
#define DATA_LEN 256

#define DEBUG 1

#include <stdint.h>
typedef uint8_t bufferMagnetDataEncoded_Type;

_Bool isEventinLectorQueue(void);
bufferMagnetDataEncoded_Type * getLectureEvent(void);
void hwLectorInit(void);



#endif //_MAGNETLECTOR_H_
