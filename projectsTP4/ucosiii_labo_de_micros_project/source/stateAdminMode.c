/*
 * stateAdminMode.c
 *
 *  Created on: Sep 1, 2019
 *      Author: Manuel Mollon
 */

#include "stateAdminMode.h"
#include "stateMenu.h"
#include "stateAddingUser.h"
#include "stateRemovingUser.h"
#include "stateReceivingPIN.h"

#include "display.h"
#include "encoder.h"
#include "dataBase.h"

#define INCREMENT	1
#define INITIAL	0

typedef enum {ADD_USER,DELETE_USER,MENU_OPTIONS}option_name;

const char * menuStrings[MENU_OPTIONS] = {"ADD","DLT"};

state_t AMinputEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	switch(ud->encoderUd.input)
	{
		case UP: // change current option
			if(ud->option < (MENU_OPTIONS-1)){
				ud->option += INCREMENT;
			}
			else{
				ud->option = INITIAL;
			}
			// show option to user
			PrintMessage(menuStrings[ud->option], false);
			break;
		case DOWN: // change current option
			if(ud->option > INITIAL){
				ud->option -= INCREMENT;
			}
			else{
				ud->option = (MENU_OPTIONS - 1); //last option
			}
			// show option to user
			PrintMessage(menuStrings[ud->option], false);
			break;
		case ENTER: // Selects current option
			switch(ud->option)
			{
				case ADD_USER:
					userDataReset(true ,true ,true ,true ,ud);
					nextState.name = ADDING_USER;
					nextState.routines[INPUT_EV] = &AUinputEvHandler;
					nextState.routines[TIMER_EV] = &AUtimerEvHandler;
					nextState.routines[KEYCARD_EV] = &AUkeycardEvHandler;
					PrintMessage("ENTER USER ID TO ADD", true);
					break;
				case DELETE_USER:
					userDataReset(true ,true ,true ,true ,ud);
					nextState.name = REMOVING_USER;
					nextState.routines[INPUT_EV] = &RUinputEvHandler;
					nextState.routines[TIMER_EV] = &RUtimerEvHandler;
					nextState.routines[KEYCARD_EV] = &RUkeycardEvHandler;
					PrintMessage("ENTER USER ID TO DELETE", true);
					break;
			}
			break;
		case CANCEL:
			userDataReset(true ,true ,true ,true ,ud);
			nextState.name = MENU;
			nextState.routines[INPUT_EV] = &MinputEvHandler;
			nextState.routines[TIMER_EV] = &MtimerEvHandler;
			nextState.routines[KEYCARD_EV] = &MkeycardEvHandler;
			PrintMessage("MENU", false);
			break;
	}
	return nextState;
}

state_t AMtimerEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY){
		UpdateDisplay();
	}
	if(ud->timerUd == INACTIVITY){
		userDataReset(true ,false ,false ,true ,ud);
		nextState.name = MENU;
		nextState.routines[INPUT_EV] = &MinputEvHandler;
		nextState.routines[TIMER_EV] = &MtimerEvHandler;
		nextState.routines[KEYCARD_EV] = &MkeycardEvHandler;
		PrintMessage("MENU", false);
		//resetear timer
	}
	return nextState;
}

state_t AMkeycardEvHandler(UserData_t * ud)
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
		// show message in display
		ud->category = verifyCategory(ud->received_ID);
		PrintMessage("VALID ID - ENTER PIN", true);
		int i;
		for(i=0;i<ID_LENGTH;++i){
			ud->received_ID[i] = cardID[i];
		}
		userDataReset(false, true, true, true, ud);
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
