/*
 * stateChangingPIN.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Manuel Mollon
 */

#ifndef CHANGING_PIN_H_
#define CHANGING_PIN_H_

#include "userData.h"
#include "FSM.h"

state_t CPinputEvHandler(UserData_t * ud);

state_t CPtimerEvHandler(UserData_t * ud);

state_t CPkeycardEvHandler(UserData_t * ud);

#endif /* CHANGING_PIN_H_ */

