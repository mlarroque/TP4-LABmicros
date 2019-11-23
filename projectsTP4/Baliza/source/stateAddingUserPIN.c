/*
 * stateAddingUserPIN.c
 *
 *  Created on: Sep 12, 2019
 *      Author: Manuel Mollon
 */

#include "stateAddingUserPIN.h"
#include "stateMenu.h"
#include "stateReceivingPIN.h"

#include "dataBase.h"
#include "display.h"
#include "encoder.h"

#define PIN_OPTIONS	13
#define LAST_OPTION_PIN	(PIN_OPTIONS-1)
#define INCREMENT	1
#define INITIAL	0
#define MAX_TRIES	3
#define HIDDEN '-'
#define STRING_CANT	(PIN_MAX_LENGTH+1)
#define INT2CHAR(x)	((char)(x+48))

typedef enum {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,BLANK,ERASE_LAST,ERASE_ALL}idOption_name;
static const char pinStrings[PIN_OPTIONS] = {'0','1','2','3','4','5','6','7','8','9',' ','L','A'};
static char PINstring[STRING_CANT];

static void createPINString(UserData_t * ud);

static void createPINString(UserData_t * ud){
	int i=0;
	while(ud->received_PIN[i] != '\0'){
		PINstring[i] = HIDDEN;
		i++;
	}
	if(ud->option != -1){
		PINstring[i] = pinStrings[ud->option];
		i++;
	}
	PINstring[i] = '\0';
}

state_t AUPinputEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	int j = 0;
	int k = 0;
	switch(ud->encoderUd.input)
	{
		case UP: // change current option
			if(ud->option < LAST_OPTION_PIN){
				ud->option += INCREMENT;
			}
			else{
				ud->option = INITIAL;
			}
			// show option to user
			createPINString(ud);
			PrintMessage(PINstring, false);
			break;
		case DOWN: // change current option
			if(ud->option > INITIAL){
				ud->option -= INCREMENT;
			}
			else{
				ud->option = LAST_OPTION_PIN;
			}
			// show option to user
			createPINString(ud);
			PrintMessage(PINstring, false);
			break;
		case ENTER: // Selects current option
			while(ud->received_PIN[j] != '\0'){
				j++;
			}
			switch(ud->option)
			{
				case ERASE_LAST:
					if(j>INITIAL)
					{
						ud->received_ID[j-1] = '\0';
					}
					userDataReset(false ,false ,false ,true ,ud);
					createPINString(ud);
					PrintMessage(PINstring, false);
					break;
				case ERASE_ALL:
					userDataReset(true ,false ,false ,true ,ud);
					createPINString(ud);
					PrintMessage(PINstring, false);
					break;
				case BLANK:
					if(j == PIN_MIN_LENGTH){
						ud->received_PIN[j] = ' ';
						user_t newUser;
						for(k = 0;k<ID_LENGTH;k++){
							newUser.usersID[k] = ud->received_ID[k];
						}
						for(k = 0;k<PIN_MAX_LENGTH;k++){
							newUser.usersPIN[k] = ud->received_PIN[k];
						}
						newUser.category = BASIC;
						switch(addUser(newUser))
						{
							case SUCCESSFULL:
								PrintMessage("NEW USER ADDED TO DATABASE", true);
								break;
							case DATABASE_FULL:
								PrintMessage("ERROR - DATABASE FULL", true);
								break;
						}
						nextState.name = MENU;
						nextState.routines[INPUT_EV] = &MinputEvHandler;
						nextState.routines[TIMER_EV] = &MtimerEvHandler;
						nextState.routines[KEYCARD_EV] = &MkeycardEvHandler;
						userDataReset(true ,true ,true ,true ,ud);
					}
					break;
				default: // number
					if((ud->option >= INITIAL) && (j < PIN_MAX_LENGTH)){
						ud->received_PIN[j] = INT2CHAR(ud->option);
						j++;
						userDataReset(false ,false ,false ,true ,ud);
						createPINString(ud);
						PrintMessage(PINstring, false);
					}
					if(j == PIN_MAX_LENGTH){ // save user
						user_t newUser;
						for(k = 0;k<ID_LENGTH;k++){
							newUser.usersID[k] = ud->received_ID[k];
						}
						for(k = 0;k<PIN_MAX_LENGTH;k++){
							newUser.usersPIN[k] = ud->received_PIN[k];
						}
						newUser.category = BASIC;
						switch(addUser(newUser))
						{
							case SUCCESSFULL:
								PrintMessage("NEW USER ADDED TO DATABASE", true);
								break;
							case DATABASE_FULL:
								PrintMessage("ERROR - DATABASE FULL", true);
								break;
						}
						nextState.name = MENU;
						nextState.routines[INPUT_EV] = &MinputEvHandler;
						nextState.routines[TIMER_EV] = &MtimerEvHandler;
						nextState.routines[KEYCARD_EV] = &MkeycardEvHandler;
						userDataReset(true ,true ,true ,true ,ud);
					}
					else{
						createPINString(ud);
						PrintMessage(PINstring, false);
					}
					break;
			}
			userDataReset(false ,false ,false ,true ,ud);
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

state_t AUPtimerEvHandler(UserData_t * ud)
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

state_t AUPkeycardEvHandler(UserData_t * ud)
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
		userDataReset(false, false, false, true, ud);
		nextState.name = RECEIVING_PIN;
		nextState.routines[INPUT_EV] = &RPinputEvHandler;
		nextState.routines[TIMER_EV] = &RPtimerEvHandler;
		nextState.routines[KEYCARD_EV] = &RPkeycardEvHandler;
		PrintMessage("ENTER PIN", false);
	}
	else{
		// show message in display
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
