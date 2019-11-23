/*
 * timer_queue.h
 *
 *  Created on: Sep 6, 2019
 *      Author: G5
 */

#ifndef TIMER_QUEUE_H_
#define TIMER_QUEUE_H_

#include "timer.h"
#include <stdbool.h>
/*****************************************************************************
 * 						DEFINICIONES Y TIPOS DE DATOS
 *****************************************************************************/
#define MAX_TIMER_EVENTS 50
typedef struct{
	timerUd_t array_of_events[MAX_TIMER_EVENTS];
	unsigned int queue_size;
}timerQueue_t;

/********************************************************************************
 * 							FUNCIONES DEL HEADER
 ********************************************************************************/

void InitializeTimerQueue(void);

//Guarda un evento en la cola, devuelve true si se pudo guardar el elemento
//exitosamente en la cola y false en caso contrario.Si la lista esta llena
//sobreescribe el ultimo evento y devuelve false.
bool PushTimerEv(timerUd_t);

//Saca el primer evento de la lista
timerUd_t PopTimerEv(void);

bool IsTimerQueueEmpty(void);



#endif /* TIMER_QUEUE_H_ */
