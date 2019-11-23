/*
 * stateReceivingID.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Manuel Mollon
 */

#ifndef RECEIVING_ID_H_
#define RECEIVING_ID_H_

#include "userData.h"
#include "FSM.h"

state_t RIinputEvHandler(UserData_t * ud);

state_t RItimerEvHandler(UserData_t * ud);

state_t RIkeycardEvHandler(UserData_t * ud);

#endif /* RECEIVING_ID_H_ */

