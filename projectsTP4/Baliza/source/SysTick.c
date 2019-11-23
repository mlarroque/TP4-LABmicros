/*
 * SysTick.c
 *
 *  Created on: 27 ago. 2019
 *      Author: G5
 */

#include "SysTick.h"
//#include "core_cm4.h"
#include "MK64F12.h"
#include "gpio.h"

#include <stdint.h>

#include <stddef.h>



#define FCLK	100000000U // Hz
#define TEST_PIN PORTNUM2PIN(PB,19)


typedef void (*sysTickFun_t)(void);

sysTickFun_t sysTickFun;

_Bool SysTick_Init (void (*funcallback)(void));

void SysTick_Handler(void);



_Bool SysTick_Init (void (*funcallback)(void))
{
	_Bool succeded = false;
	if (funcallback != NULL)
	{
		SysTick->CTRL = 0x00; 								// enable sysTick interrupts
		SysTick->LOAD = (FCLK/SYSTICK_ISR_FREQ_HZ) - 1; 	// load value = (pulses per period) - 1
		SysTick->VAL=0x00;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk| SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
		sysTickFun = funcallback;
		NVIC_EnableIRQ(SysTick_IRQn);
		gpioMode(TEST_PIN, OUTPUT);
		succeded = true;
	}

	return succeded;
}


void SysTick_Handler(void)
{
	gpioWrite(TEST_PIN, HIGH); //Prendo el pin de testeo cuando inicializa el callback
	sysTickFun();
	gpioWrite(TEST_PIN, LOW); //Lo apago al terminar el callback.
	 //para sysTick no hace falta realizar un "clear" de ningún flag de la interrupción.
}

