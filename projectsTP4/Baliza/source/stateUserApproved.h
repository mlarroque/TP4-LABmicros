/*
 * stateUserApproved.h
 *
 *  Created on: Sep 5, 2019
 *      Author: Manuel Mollon
 */

#ifndef STATEUSERAPPROVED_H_
#define STATEUSERAPPROVED_H_

#include "userData.h"
#include "FSM.h"

state_t UAinputEvHandler(UserData_t * ud);

state_t UAtimerEvHandler(UserData_t * ud);

state_t UAkeycardEvHandler(UserData_t * ud);

#endif /* STATEUSERAPPROVED_H_ */
