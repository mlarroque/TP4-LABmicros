/*
 * cardLectorInputEvents.h
 *
 *  Created on: 5 sep. 2019
 *      Author: G5
 */

#ifndef MAGNETCARDLECTOR_H_
#define MAGNETCARDLECTOR_H_
#define MAX_TRACK_LEN 200

typedef struct{

	int trackNum;
	char trackString[MAX_TRACK_LEN];
	_Bool isValid;

}magnetLectorData_t;

void initMagnetCardLector(void);
_Bool someMagnetCard2Read(void);
magnetLectorData_t getLectureDecoded(void);


#endif /* MAGNETCARDLECTOR_H_ */
