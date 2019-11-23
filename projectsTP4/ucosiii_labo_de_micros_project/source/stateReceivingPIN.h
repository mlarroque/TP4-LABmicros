/*
 * stateReceivingPIN.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Manuel Mollon
 */

#ifndef RECEIVING_PIN_H_
#define RECEIVING_PIN_H_

#include "userData.h"
#include "FSM.h"

state_t RPinputEvHandler(UserData_t * ud);

state_t RPtimerEvHandler(UserData_t * ud);

state_t RPkeycardEvHandler(UserData_t * ud);

#endif /* RECEIVING_PIN_H_ */

