 /*
 * stateMenu.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Manuel Mollon
 */

#ifndef MENU_H_
#define MENU_H_

#include "userData.h"
#include "FSM.h"

state_t MinputEvHandler(UserData_t * ud);

state_t MtimerEvHandler(UserData_t * ud);

state_t MkeycardEvHandler(UserData_t * ud);

#endif /* MENU_H_ */

