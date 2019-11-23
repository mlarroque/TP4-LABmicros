/*
 * stateChangeIntensity.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Manuel Mollon
 */

#ifndef CHANGE_INTENSITY_H_
#define CHANGE_INTENSITY_H_

#include "userData.h"
#include "FSM.h"

state_t CIinputEvHandler(UserData_t * ud);

state_t CItimerEvHandler(UserData_t * ud);

state_t CIkeycardEvHandler(UserData_t * ud);

#endif /* CHANGE_INTENSITY_H_ */

