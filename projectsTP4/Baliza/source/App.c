/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include "FSM.h"
#include "queue.h"
#include "dataBase.h"


//IO
#include "display.h"
#include "encoder.h"
#include "magnetCardLector.h"
#include "timer.h"
#include "timer_queue.h"
#include "doorManagement.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define INACTIVITY_TIME 60000 //Timeout de inactividad en milisegundos.


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// FSM.h
static FSM_t fsm;
static state_t nextState;
static UserData_t userData;
// App.c
static bool changingState = false;
static ev_name nameEvent;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void inactCallback(){
	PushTimerEv(INACTIVITY);
}

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	// IO Initialization
    InitializeTimers();
    SetTimer(INACTIVITY,INACTIVITY_TIME,inactCallback);
    initializeEncoder();
    InitializeDisplay();
    initMagnetCardLector();
    doorInit();


    // FSM/Queue Initialization
    initFSM(&fsm);
    initializeDataBase();

    // User Data init
    userDataReset(true, true, true, true, &userData);

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	nameEvent = getEvent(&userData); // get new event
	switch(nameEvent){ // which type of event?
		case INPUT_EV:
			RestartTimer(INACTIVITY);
			nextState = (fsm.currentState.routines[INPUT_EV])(&userData); // action routine
			if(nextState.name != STAY){ // if state changes
				changingState = true;
			}
			break;
		case TIMER_EV:
			nextState = (fsm.currentState.routines[TIMER_EV])(&userData); // action routine
			if(nextState.name != STAY){ // if state changes
				changingState = true;
			}
			break;
		case KEYCARD_EV:
			RestartTimer(INACTIVITY);
			nextState = (fsm.currentState.routines[KEYCARD_EV])(&userData); // action routine
			if(nextState.name != STAY){ // if state changes
				changingState = true;
			}
			break;
		default:
			break;
	}
	if(changingState){
		fsm.currentState = nextState;
		changingState = false;
	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 ******************************************************************************/
