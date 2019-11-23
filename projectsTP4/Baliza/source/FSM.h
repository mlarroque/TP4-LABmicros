/*
 * FSM.h
 *
 * Header con las definiciones de las estructuras utilizadas
 * para los  estados, eventos y la FSM.
 *
 *  Created on: Aug 28, 2019
 *      Author: Ezequiel Vijande
 */

#ifndef FSM_H_
#define FSM_H_

#include "queue.h"
#include <stdbool.h>

typedef enum {MENU, CHANGE_INTENSITY, RECEIVING_ID, RECEIVING_PIN, ADMIN_MODE, ADDING_USER, ADDING_USER_PIN,
			REMOVING_USER, CHANGING_PIN,USER_APPROVED, BLOCKED, NUM_STATES,STAY}state_name; //Estados posibles de la FSM

struct state;
typedef struct state (*StateRoutinePtr_t)(UserData_t *);

typedef struct state{
	state_name name;
	StateRoutinePtr_t routines[NUM_EVENTS]; //La cantidad de rutinas es igual a la cantidad de eventos posibles.
}state_t;

typedef struct{
	state_t currentState;
	bool exit;
}FSM_t;

void initFSM(FSM_t * fsm);

#endif /* FSM_H_ */
