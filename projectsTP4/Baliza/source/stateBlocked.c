/*
 * stateBlocked.c
 *
 *  Created on: Sep 1, 2019
 *      Author: Manuel Mollon
 */

#include "stateBlocked.h"
#include "stateReceivingPIN.h"

#include "display.h"
#include "encoder.h"
#include "timer.h"

state_t BinputEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	return nextState;
}

state_t BtimerEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY){
		UpdateDisplay();
	}
	if(ud->timerUd == UNBLOCKED){
		userDataReset(false ,true ,false ,true ,ud);
		nextState.name = RECEIVING_PIN;
		nextState.routines[INPUT_EV] = &RPinputEvHandler;
		nextState.routines[TIMER_EV] = &RPtimerEvHandler;
		nextState.routines[KEYCARD_EV] = &RPkeycardEvHandler;
		PrintMessage("BLOCKED TIME OVER", true);
	}
	return nextState;
}

state_t BkeycardEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	return nextState;
}

