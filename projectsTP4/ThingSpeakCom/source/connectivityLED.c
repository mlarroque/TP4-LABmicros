/*
 * connectivityLED.c
 *
 *  Created on: Nov 21, 2019
 *      Author: Manuel Mollon
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "connectivityLED.h"
#include "gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// On Board User LEDs
#define PIN_LED_RED     PORTNUM2PIN(PB,22)		// PTB22
#define PIN_LED_GREEN   PORTNUM2PIN(PE,26)		// PTE26
#define PIN_LED_BLUE    PORTNUM2PIN(PB,21) 		// PTB21

/*******************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 ******************************************************************************/

void connectivityLedInit(void){
    gpioMode(PIN_LED_RED, OUTPUT);
    gpioMode(PIN_LED_GREEN, OUTPUT);
}

void updateConnectivityLed(bool connected){
	gpioWrite(PIN_LED_RED, HIGH);
	gpioWrite(PIN_LED_GREEN, HIGH);
	if(connected){
		gpioWrite(PIN_LED_GREEN, LOW);
	}
	else{
		gpioWrite(PIN_LED_RED, LOW);
	}
}



