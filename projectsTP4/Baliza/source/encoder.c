/*
 * encoder.c
 *
 *  Created on: Sep 1, 2019
 *      Author: Lu
 */

#include "encoder.h"
#include "encoderDecoder.h"
#include "encoderHAL.h"

/******************************************************************************
 *									DEFINICIONES
 ******************************************************************************/
#define LOW 0
#define HIGH 1

typedef struct{
	int top; 			//contador de eventos
	encoderUd_t event;
	_Bool isEmpty;
}encoderQueue_t;

#define ENCODER_EVENTS		200

#define CANCEL_COUNT		20			//el tiempo que tiene que pasar para que sea evento CANCEL es CANCEL_COUNT*ENCODER_TIME (=200ms*20)
//#define BACK_COUNT		40			//entre .5 y 2 segundos para que sea evento = BACK
#define ENTER_COUNT			10			//<.5 segundos para que el evento sea = ENTER

/*******************************************************************************
 *								VARIABLES ESTATICAS
 *******************************************************************************/

static _Bool initialized_enc = false;
static encoderQueue_t encoderQueue[ENCODER_EVENTS];

/*******************************************************************************
 * 								FUNCIONES LOCALES
 *******************************************************************************/

void setEncCallbacks(void);


void buttonCallback(void);
void rotationCallback(void);


void pushEncoderEvent(encoderUd_t ev);
encoderUd_t popEncoderEvent(void);
void initializeEncoderQueue(void);
encoderQueue_t getEncoderEvent(void);

/********************************************************************************
 * 							FUNCIONES DEL HEADER
 ********************************************************************************/

void initializeEncoder(void)
{
	if(!initialized_enc)
	{
		uint8_t i;
		uint8_t j;
		initializeEncoderHAL(rotationCallback);		//setea gpio y timer count
		setButtonCallback(buttonCallback);
		initializeEncoderQueue();			//inicializo queue de encoder

		for(j=0; j<STATES;j++)
			for(i=0;i<ENC_SIGNAL_COUNT;i++)
				updateData(readEncoderSignalX(i), i);				//inicializo estructura encoder_t con las señales en el instante actual y el anterior

		initialized_enc = true;
	}
}

void rotationCallback(void)
{
	updateData(readEncoderSignalX(A), A);
	updateData(readEncoderSignalX(B), B);
	encoderUd_t eventForQueue;
	eventForQueue.isValid = false;
	eventForQueue.input = NO_CHANGE;
	counter_type event = NO_CHANGE;
	event = decodeEncoder();

	if(event == COUNT_UP)
	{
		eventForQueue.input = UP;
		eventForQueue.isValid = true;
		pushEncoderEvent(eventForQueue);
	}
	else if(event == COUNT_DOWN)
	{
		eventForQueue.input = DOWN;
		eventForQueue.isValid = true;
		pushEncoderEvent(eventForQueue);
	}
	else if(event == RESET)
	{
		eventForQueue.isValid = false;
		resetData();
	}
	else
	{
		eventForQueue.isValid = false;
	}

}

void buttonCallback(void)
{
	updateData(readEncoderSignalX(C), C);
	encoderQueue_t eventForQueue;
	eventForQueue.event.input = NO_ENC_EVENT;
 	eventForQueue.event.isValid = false;
	if(checkEnterFallingEdge())				//si fue flanco descendente recién se presionó el botón
	{
		resetEncoderTimerCount();			//reseteo el contador
	}
	else if(checkEnterRisingEdge())		//si fue un flanco descendente me fijo cuánto tiempo se presionó el botón para saber si fue ENTER; BACK o CANCEL
	{
		if(getEncTimerCount() < ENTER_COUNT)		//si es menor a ENTER_COUNT el evento es ENTER
		{
			eventForQueue.event.input = ENTER;
			eventForQueue.event.isValid = true;
			pushEncoderEvent(eventForQueue.event);
		}
		//else if(getEncTimerCount() < BACK_COUNT)	//si es menos de BACK_COUNT o mas de ENTER_COUNT el evento es BACK
		//{
		//	eventForQueue.event.input = BACK;
		//	eventForQueue.event.isValid = true;
		//	pushEncoderEvent(eventForQueue.event);
		//}
		else		//si fue más de BACK_COUNT, tomó que fue evento = CANCEL
		{
			eventForQueue.event.input = CANCEL;
			eventForQueue.event.isValid = true;
			pushEncoderEvent(eventForQueue.event);
		}
	}
}




void initializeEncoderQueue(void)
{
	encoderQueue->top = -1;
	encoderQueue->isEmpty = true;
}

encoderUd_t popEncoderEvent(void)
{
	encoderUd_t poppedEvent;
	if(encoderQueue->top == -1)
	{ // queue empty -> top = -1
		encoderQueue->isEmpty = true;
		poppedEvent.isValid = false;
	}
	else
	{
		poppedEvent = encoderQueue[encoderQueue->top].event; //popEvent
		poppedEvent.isValid = true;
		encoderQueue->top -= 1; // Decrement queue counter
		if(encoderQueue->top == -1)
		{
			encoderQueue->isEmpty = true;
		}
	}
	return poppedEvent;
}

_Bool isEncEventValid(void)
{
	return !encoderQueue->isEmpty;
}
void pushEncoderEvent(encoderUd_t ev)
{
	if(encoderQueue->top == ENCODER_EVENTS-1)
	{ // event overflow
		encoderQueue->top = 0;
		encoderQueue[encoderQueue->top].event = ev;
		encoderQueue->isEmpty = false;
	}
	else{
		encoderQueue->top += 1;
		encoderQueue[encoderQueue->top].event = ev;
		encoderQueue->isEmpty = false;
	}
	return;
}




