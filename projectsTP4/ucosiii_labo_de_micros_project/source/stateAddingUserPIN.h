/*
 * stateAddingUserPIN.h
 *
 *  Created on: Sep 12, 2019
 *      Author: Manuel Mollon
 */

#ifndef STATEADDINGUSERPIN_H_
#define STATEADDINGUSERPIN_H_

#include "userData.h"
#include "FSM.h"

state_t AUPinputEvHandler(UserData_t * ud);

state_t AUPtimerEvHandler(UserData_t * ud);

state_t AUPkeycardEvHandler(UserData_t * ud);

#endif /* STATEADDINGUSERPIN_H_ */
