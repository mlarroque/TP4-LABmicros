/*
 * stateReceivingPIN.c
 *
 *  Created on: Sep 1, 2019
 *      Author: Manuel Mollon
 */

#include "stateReceivingPIN.h"
#include "stateBlocked.h"
#include "stateMenu.h"
#include "stateUserApproved.h"

#include "display.h"
#include "encoder.h"
#include "dataBase.h"
#include "timer_queue.h"
#include <stdbool.h>

#define PIN_OPTIONS	13
#define LAST_OPTION_PIN	(PIN_OPTIONS-1)
#define INCREMENT	1
#define INITIAL	0
#define MAX_TRIES	3
#define HIDDEN '-'
#define STRING_CANT	(PIN_MAX_LENGTH+1)
#define INT2CHAR(x)	((char)(x+48))
#define BLOCKED_TIME	60000UL // 1 min in ms

typedef enum {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,BLANK,ERASE_LAST,ERASE_ALL}idOption_name;
static const char pinStrings[PIN_OPTIONS] = {'0','1','2','3','4','5','6','7','8','9',' ','L','A'};
static char PINstring[STRING_CANT];

static void createPINString(UserData_t * ud);

static long unsigned int getBlockedTime(void);

void blockedCallback(void);

void blockedCallback(){
	PushTimerEv(UNBLOCKED);
	DisableTimer(UNBLOCKED);
	RestartTimer(INACTIVITY);
}

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

static int tryNro = 0;
static long unsigned int f1=0;
static long unsigned int f2=1;

static long unsigned int getBlockedTime(void)
{
	static long unsigned int f3;
	f3 = f2+f1;
	f1 = f2;
	f2 = f3;
	return f3*BLOCKED_TIME;
}

state_t RPinputEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	int j = 0;
	bool validPIN = false;
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
						ud->received_PIN[j-1] = '\0';
					}
					userDataReset(false ,false ,false ,true ,ud);
					createPINString(ud);
					PrintMessage(PINstring, false);
					break;
				case ERASE_ALL:
					userDataReset(false ,true ,false ,true ,ud);
					createPINString(ud);
					PrintMessage(PINstring, false);
					break;
				case BLANK:
					if(j == PIN_MIN_LENGTH){
						ud->received_PIN[j] = ' ';
						validPIN = verifyPIN(ud->received_ID,ud->received_PIN);
						if(validPIN){
							nextState.name = USER_APPROVED;
							tryNro = 0;
							nextState.routines[INPUT_EV] = &UAinputEvHandler;
							nextState.routines[TIMER_EV] = &UAtimerEvHandler;
							nextState.routines[KEYCARD_EV] = &UAkeycardEvHandler;
							PrintMessage("USER APPROVED", true);
						}
						else
						{
							PrintMessage("INCORRECT PIN", true);
							userDataReset(false ,true ,false ,true ,ud);
							tryNro ++;
							if(tryNro < MAX_TRIES){
								//stays
							}
							else{
								nextState.name = BLOCKED;
								nextState.routines[INPUT_EV] = &BinputEvHandler;
								nextState.routines[TIMER_EV] = &BtimerEvHandler;
								nextState.routines[KEYCARD_EV] = &BkeycardEvHandler;
								PrintMessage("USER BLOCKED", true);
								SetTimer(UNBLOCKED,getBlockedTime(),blockedCallback);
							}
						}
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
					if(j == PIN_MAX_LENGTH){ // check if pin valid
						validPIN = verifyPIN(ud->received_ID, ud->received_PIN);
						if(validPIN){
							nextState.name = USER_APPROVED;
							tryNro = 0;
							nextState.routines[INPUT_EV] = &UAinputEvHandler;
							nextState.routines[TIMER_EV] = &UAtimerEvHandler;
							nextState.routines[KEYCARD_EV] = &UAkeycardEvHandler;
							PrintMessage("USER APPROVED", true);
						}
						else
						{
							PrintMessage("INCORRECT PIN", true);
							userDataReset(false ,true ,false ,true ,ud);
						    tryNro ++;
						    if(tryNro < MAX_TRIES){
						    	//	stays
						    }
						    else{
 								nextState.name = BLOCKED;
								nextState.routines[INPUT_EV] = &BinputEvHandler;
								nextState.routines[TIMER_EV] = &BtimerEvHandler;
								nextState.routines[KEYCARD_EV] = &BkeycardEvHandler;
								PrintMessage("USER BLOCKED", true);
								SetTimer(UNBLOCKED,getBlockedTime(),blockedCallback);
						    }
						}
					}
					break;
			}
			userDataReset(false ,false ,false ,true ,ud);
			break;
		case CANCEL:
			userDataReset(true ,true ,true ,true ,ud);
		    tryNro = 0;
			nextState.name = MENU;
			nextState.routines[INPUT_EV] = &MinputEvHandler;
			nextState.routines[TIMER_EV] = &MtimerEvHandler;
			nextState.routines[KEYCARD_EV] = &MkeycardEvHandler;
			PrintMessage("MENU", false);
			break; // Cancels selection and back to menu
	}
	return nextState;
}

state_t RPtimerEvHandler(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY){
		UpdateDisplay();
	}
	if(ud->timerUd == INACTIVITY){
		userDataReset(true ,true ,false ,true ,ud);
		tryNro = 0;
		nextState.name = MENU;
		nextState.routines[INPUT_EV] = &MinputEvHandler;
		nextState.routines[TIMER_EV] = &MtimerEvHandler;
		nextState.routines[KEYCARD_EV] = &MkeycardEvHandler;
		PrintMessage("MENU", false);
		//resetear timer
	}
	return nextState;
}

state_t RPkeycardEvHandler(UserData_t * ud)
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
		PrintMessage("VALID ID - ENTER PIN", true);
		tryNro = 0;
		ud->category = verifyCategory(ud->received_ID);
		int i;
		for(i=0;i<ID_LENGTH;++i){
			ud->received_ID[i] = cardID[i];
		}
		userDataReset(false, true, false, true, ud);
		nextState.name = RECEIVING_PIN;
		nextState.routines[INPUT_EV] = &RPinputEvHandler;
		nextState.routines[TIMER_EV] = &RPtimerEvHandler;
		nextState.routines[KEYCARD_EV] = &RPkeycardEvHandler;
	}
	else{
		// show message in display
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
