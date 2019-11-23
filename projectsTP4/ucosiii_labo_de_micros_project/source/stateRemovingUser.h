/*
 * stateRemovingUser.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Manuel Mollon
 */

#ifndef REMOVING_USER_H_
#define REMOVING_USER_H_

#include "userData.h"
#include "FSM.h"

state_t RUinputEvHandler(UserData_t * ud);

state_t RUtimerEvHandler(UserData_t * ud);

state_t RUkeycardEvHandler(UserData_t * ud);

#endif /* REMOVING_USER_H_ */

