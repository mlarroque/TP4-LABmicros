/*
 * timer_queue.c
 *
 *  Created on: Sep 6, 2019
 *      Author: G5
 */
#include "timer_queue.h"
/**********************************************************************
 *						VARIABLES ESTATICAS
 **********************************************************************/
static timerQueue_t queue;
static bool tqInitialized = false;
/**********************************************************************
 * 						FUNCIONES DEL HEADER
 **********************************************************************/
void InitializeTimerQueue(void)
{
	if(!tqInitialized)
	{
		queue.queue_size = 0;
	}
}

bool PushTimerEv(timerUd_t ev)
{
	bool success = false;
	if(queue.queue_size < MAX_TIMER_EVENTS-1)
	{
		(queue.array_of_events)[queue.queue_size] = ev;
		(queue.queue_size)++;
		success = true;
	}
	else
	{
		(queue.array_of_events)[MAX_TIMER_EVENTS-1] = ev;
	}
	return success;
}

timerUd_t PopTimerEv(void)
{
	return (queue.array_of_events)[--(queue.queue_size)];
}

bool IsTimerQueueEmpty(void)
{
	bool result;
	if(queue.queue_size == 0)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}

