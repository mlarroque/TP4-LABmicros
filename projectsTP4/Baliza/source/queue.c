/*
 * queue.c
 *
 *  Created on: Sep 1, 2019
 *      Author: Manuel Mollon
 */

#include "queue.h"

#include "timer.h"
#include "timer_queue.h"
#include "encoder.h"
#include "magnetCardLector.h"

ev_name getEvent(UserData_t * ud)
{
	ev_name name = NUM_EVENTS;
	if(!IsTimerQueueEmpty()){
		ud->timerUd = PopTimerEv();
		name = TIMER_EV;
	}
	else if(someMagnetCard2Read()){
		ud->magnetLectorUd = getLectureDecoded(); // get specific data for that event
		name = KEYCARD_EV;
	}
	else if(isEncEventValid()){
		ud->encoderUd = popEncoderEvent();
		name = INPUT_EV;
	}
	return name;
}

void userDataReset(bool resetID, bool resetPIN, bool resetCategory, bool resetOption, UserData_t * ud)
{
	int i;
	if(resetID){
	    for(i=0;i<ID_LENGTH;++i){
	    	ud->received_ID[i] = '\0';
	    } // clean user ID
	}
	if(resetPIN){
	    for(i=0;i<PIN_MAX_LENGTH;++i){
	    	ud->received_PIN[i] = '\0';
	    } // clean user PIN
	}
	if(resetCategory){
		ud->category = NONE;
	}
	if(resetOption){
		ud->option = -1;
	}
}

