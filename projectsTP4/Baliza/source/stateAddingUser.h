/*
 * stateAddingUser.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Manuel Mollon
 */

#ifndef ADDING_USER_H_
#define ADDING_USER_H_

#include "userData.h"
#include "FSM.h"

state_t AUinputEvHandler(UserData_t * ud);

state_t AUtimerEvHandler(UserData_t * ud);

state_t AUkeycardEvHandler(UserData_t * ud);

#endif /* ADDING_USER_H_ */
