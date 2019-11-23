/*
 * stateBlocked.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Manuel Mollon
 */

#ifndef BLOCKED_H_
#define BLOCKED_H_

#include "userData.h"
#include "FSM.h"

state_t BinputEvHandler(UserData_t * ud);

state_t BtimerEvHandler(UserData_t * ud);

state_t BkeycardEvHandler(UserData_t * ud);

#endif /* BLOCKED_H_ */

