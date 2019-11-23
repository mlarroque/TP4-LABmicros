/*
 * stateUserApproved.c
 *
 *  Created on: Sep 5, 2019
 *      Author: Manuel Mollon
 */

#include "stateUserApproved.h"
#include "stateAdminMode.h"
#include "stateChangingPIN.h"
#include "stateReceivingPIN.h"
#include "stateMenu.h"

#include "doorManagement.h"
#include "dataBase.h"
#include "display.h"
#include "encoder.h"

#define INCREMENT	1
#define INITIAL	0

typedef enum {GRANT_ACCESS,CHANGE_PIN,ADMIN_OPTION,MENU_OPTIONS}option_name;
static const char * menuStrings[MENU_OPTIONS] = {"OPEN","PIN","ADM"};

state_t UAinputEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	char category;
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
				ud->option = (MENU_OPTIONS-1); //last option
			}
			// show option to user
			PrintMessage(menuStrings[ud->option], false);
			break;
		case ENTER: // Selects current option
			switch(ud->option)
			{
				case GRANT_ACCESS:
					userDataReset(true ,true ,true ,true ,ud);
					nextState.name = MENU;
					nextState.routines[INPUT_EV] = &MinputEvHandler;
					nextState.routines[TIMER_EV] = &MtimerEvHandler;
					nextState.routines[KEYCARD_EV] = &MkeycardEvHandler;
					PrintMessage("MENU", false);
					openDoorTemporally();
					break;
				case CHANGE_PIN:
					userDataReset(false ,true ,false ,true ,ud);
					nextState.name = CHANGING_PIN;
					nextState.routines[INPUT_EV] = &CPinputEvHandler;
					nextState.routines[TIMER_EV] = &CPtimerEvHandler;
					nextState.routines[KEYCARD_EV] = &CPkeycardEvHandler;
					PrintMessage("ENTER NEW PIN", true);
					break;
				case ADMIN_OPTION:
					category = verifyCategory(ud->received_ID);
					if(category == ADMIN){
						userDataReset(false ,false ,false ,true ,ud);
						nextState.name = ADMIN_MODE;
						nextState.routines[INPUT_EV] = &AMinputEvHandler;
						nextState.routines[TIMER_EV] = &AMtimerEvHandler;
						nextState.routines[KEYCARD_EV] = &AMkeycardEvHandler;
						PrintMessage("ADMIN MENU", true);
					}
					else{
						PrintMessage("ACCESS DENIED - USER NOT ADMIN", true);
					}
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

state_t UAtimerEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY){
		UpdateDisplay();
	}
	if(ud->timerUd == INACTIVITY){
		userDataReset(true ,true ,false ,true ,ud);
		nextState.name = MENU;
		nextState.routines[INPUT_EV] = &MinputEvHandler;
		nextState.routines[TIMER_EV] = &MtimerEvHandler;
		nextState.routines[KEYCARD_EV] = &MkeycardEvHandler;
		PrintMessage("MENU", false);
		//resetear timer
	}
	return nextState;
}

state_t UAkeycardEvHandler(UserData_t * ud)
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
		userDataReset(false, true, false, true, ud);
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
