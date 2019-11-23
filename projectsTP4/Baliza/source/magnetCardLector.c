/*
 * magnetCardLector.c
 *
 *  Created on: 6 sep. 2019
 *      Author: G5
 */

#include "magnetCardLector.h"
#include "magnetLectorHAL.h"
#include "magnetCardDecode.h"
#include <stdbool.h>

#include "gpio.h" ////esto es para debuguear

#define DEBUG_DECODING_PIN PORTNUM2PIN(PE, 25) //cable naranja (PE25: arriba a la derecha)

void initMagnetCardLector(void){
	hwLectorInit();
	if(DEBUG)
	{
		gpioMode(DEBUG_DECODING_PIN, OUTPUT);
		gpioWrite(DEBUG_DECODING_PIN, LOW);
	}
}

_Bool someMagnetCard2Read(void){
	return isEventinLectorQueue();
}
magnetLectorData_t getLectureDecoded(void){
	magnetLectorData_t ret;
	if(isEventinLectorQueue())
	{
		if(DEBUG)
		{
			gpioWrite(DEBUG_DECODING_PIN, HIGH);
		}
		if(magnetDataParser(getLectureEvent(), ret.trackString, &(ret.trackNum)))
		{
			ret.isValid = true;
		}
		else
		{
			ret.isValid = false;
		}
		if(DEBUG)
		{
			gpioWrite(DEBUG_DECODING_PIN, LOW);
		}
	}
	return ret;
}


