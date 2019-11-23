/*
 * stateChangeIntensity.c
 *
 *  Created on: Sep 1, 2019
 *      Author: Manuel Mollon
 */

#include "stateChangeIntensity.h"
#include "stateMenu.h"
#include "stateReceivingPIN.h"

#include "display.h"
#include "encoder.h"
#include "dataBase.h"

#define INTENSITY_OPTIONS	4
#define INCREMENT	1
#define INITIAL	 0
#define LAST_OPTION (INTENSITY_OPTIONS-1)

const char * intStrings[INTENSITY_OPTIONS] = {"25","50","75","100"};


state_t CIinputEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	switch(ud->encoderUd.input)
	{
		case UP: // change current option
			if(ud->option < LAST_OPTION){
				ud->option += INCREMENT;
			}
			else{
				ud->option = INITIAL;
			}
			// show option to user
			PrintMessage(intStrings[ud->option], false);
			break;
		case DOWN: // change current option
			if(ud->option > INITIAL){
				ud->option -= INCREMENT;
			}
			else{
				ud->option = LAST_OPTION;
			}
			// show option to user
			PrintMessage(intStrings[ud->option], false);
			break;
		case ENTER: // Selects current option
			if(ud->option < INITIAL)
			{
				PrintMessage("PLEASE SELECT INTENSITY FIRST", true);
			}
			else
			{
				SetBrightness((unsigned char) (ud->option+1));
				userDataReset(false, false, false, true, ud);
				nextState.name = MENU;
				nextState.routines[INPUT_EV] = &MinputEvHandler;
				nextState.routines[TIMER_EV] = &MtimerEvHandler;
				nextState.routines[KEYCARD_EV] = &MkeycardEvHandler;
				PrintMessage("MENU", false);
			}
			break;
		case CANCEL:
			userDataReset(true ,true ,true ,true ,ud);
			nextState.name = MENU;
			nextState.routines[INPUT_EV] = &MinputEvHandler;
			nextState.routines[TIMER_EV] = &MtimerEvHandler;
			nextState.routines[KEYCARD_EV] = &MkeycardEvHandler;
			PrintMessage("MENU", false);
			break; // Cancels selection and back to menu
	}
	return nextState;
}

state_t CItimerEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY){
		UpdateDisplay();
	}
	return nextState;
}

state_t CIkeycardEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	char cardID[ID_LENGTH];
	int i;
	for(i=0;i<ID_LENGTH;++i){
		cardID[i] = ud->magnetLectorUd.trackString[i];
	}
	bool IDExists = verifyID(cardID);
	if(IDExists){
		ud->category = verifyCategory(ud->received_ID);
		// show message in display
		PrintMessage("VALID ID - ENTER PIN", true);
		int i;
		for(i=0;i<ID_LENGTH;++i){
			ud->received_ID[i] = cardID[i];
		}
		userDataReset(false, false, false, true, ud);
		nextState.name = RECEIVING_PIN;
		nextState.routines[INPUT_EV] = &RPinputEvHandler;
		nextState.routines[TIMER_EV] = &RPtimerEvHandler;
		nextState.routines[KEYCARD_EV] = &RPkeycardEvHandler;
		PrintMessage("ENTER PIN", true);
	}
	else{
		// show message in display
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
