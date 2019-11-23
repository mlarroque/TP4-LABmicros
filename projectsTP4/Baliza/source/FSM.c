/*
 * FSM.c
 *
 *  Created on: Sep 6, 2019
 *      Author: Manuel Mollon
 */

#include "FSM.h"
#include "stateMenu.h"
#include "display.h"

void initFSM(FSM_t * fsm)
{
	fsm->currentState.name = MENU;
	fsm->currentState.routines[INPUT_EV] = &MinputEvHandler;
	fsm->currentState.routines[TIMER_EV] = &MtimerEvHandler;
	fsm->currentState.routines[KEYCARD_EV] = &MkeycardEvHandler;
	PrintMessage("MENU", false);
}
