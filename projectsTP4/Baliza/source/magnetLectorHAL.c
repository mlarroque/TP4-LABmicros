//magnetLectorDriver.c

#include "magnetLectorHAL.h"
#include "gpio.h"
#include <stddef.h>

#define CLOCK_PIN PORTNUM2PIN(PC, 9) //cable verde
#define DATA_PIN PORTNUM2PIN(PC, 7)  //cable azul
#define ENABLE_PIN PORTNUM2PIN(PC, 5) //cable amarillo


#define DEBUG_FALL_ENABLE_PIN PORTNUM2PIN(PC, 8) //cable blanco
#define DEBUG_RISE_ENABLE_PIN PORTNUM2PIN(PC, 1) //cable gris
#define DEBUG_FALL_CLOCK_PIN PORTNUM2PIN(PB, 18) //cable marrón




typedef struct{
	int top;
	bufferMagnetDataEncoded_Type encodedWords[MAX_ITEMS_LECTOR_QUEUE][DATA_LEN];
	_Bool isEmpty;
}magnetLectorQueue_t;


void clockRoutine(void);

void enableRoutine(void);

void debugSwitchRoutine(void);

void initLectorQueue(void);


_Bool hwLectorIsInit = false;
_Bool enable = false;

uint8_t raw2save = 0;
uint32_t counterDataIn = 0;


static magnetLectorQueue_t queue;


void hwLectorInit(void)
{
	if (!hwLectorIsInit)
	{

		gpioMode(CLOCK_PIN , INPUT_PULLUP);  //inicialización de señales del lector
		gpioMode(DATA_PIN, INPUT_PULLUP);
		gpioMode(ENABLE_PIN, INPUT_PULLUP);

		gpioIRQ(ENABLE_PIN, GPIO_IRQ_MODE_BOTH_EDGES, (pinIrqFun_t) enableRoutine);
		enable = false;
		if(DEBUG)
		{
			/*gpioMode(DEBUG_FALL_ENABLE_PIN, OUTPUT);
			gpioMode(DEBUG_RISE_ENABLE_PIN, OUTPUT);
			gpioMode(DEBUG_FALL_CLOCK_PIN, OUTPUT);
			gpioWrite(DEBUG_FALL_ENABLE_PIN, LOW);
			gpioWrite(DEBUG_RISE_ENABLE_PIN, LOW);
			gpioWrite(DEBUG_FALL_CLOCK_PIN, LOW);*/
		}
	}
	initLectorQueue();
}

void clockRoutine(void)
{
	if(DEBUG)
	{
		gpioWrite(DEBUG_FALL_CLOCK_PIN, HIGH);
	}
	if(counterDataIn < DATA_LEN)  //si queda lugar en el buffer, sigo ingresando datos
	{
		queue.encodedWords[raw2save][counterDataIn] = !gpioRead(DATA_PIN);  //activo bajo
		counterDataIn++;
	}
	if(DEBUG)
	{
		gpioWrite(DEBUG_FALL_CLOCK_PIN, LOW);
	}

}

void enableRoutine(void)
{
	enable = !enable;
	if(enable) //se comienza a leer la tarjeta
	{
		if(DEBUG)
		{
			gpioWrite(DEBUG_FALL_ENABLE_PIN, HIGH);
		}
		counterDataIn = 0;
		gpioIRQ(CLOCK_PIN, GPIO_IRQ_MODE_FALLING_EDGE, (pinIrqFun_t) clockRoutine);
		if(DEBUG)
		{
			gpioWrite(DEBUG_FALL_ENABLE_PIN, LOW);
		}
	}
	else
	{
		if(DEBUG)
		{
			gpioWrite(DEBUG_RISE_ENABLE_PIN, HIGH);
		}
		gpioIRQ(CLOCK_PIN, GPIO_IRQ_MODE_DISABLE, (pinIrqFun_t) clockRoutine);
		queue.top = raw2save;
		queue.isEmpty = false;
		if(raw2save < (MAX_ITEMS_LECTOR_QUEUE - 1))
		{
			raw2save++;
		}
		 //OSQPost(&AppQ, (void *)&data, 4, OS_OPT_POST_ALL, &os_err);
		//aca hay que habilitar el semáforo.
		if(DEBUG)
		{
			gpioWrite(DEBUG_RISE_ENABLE_PIN, LOW);
		}
	}
}

bufferMagnetDataEncoded_Type * getLectureEvent(void)
{
	bufferMagnetDataEncoded_Type * ret;
	if(queue.isEmpty){
		ret = NULL;
	}
	else
	{   //popEncodedWord
		ret = &(queue.encodedWords[queue.top][0]);
		if(queue.top == 0)
		{
			queue.isEmpty = true;
			raw2save = 0;
		}
		else
		{
			queue.top--;
			raw2save--;
		}
	}
	return ret;
}

_Bool isEventinLectorQueue(void)
{
	return !queue.isEmpty;
}

void initLectorQueue(void)
{
	queue.isEmpty = true;
	queue.top = 0;
}

