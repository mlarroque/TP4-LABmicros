/*
 * stateAdminMode.h
 *
 *  Created on: Aug 31, 2019
 *      Author: Manuel Mollon
 */

#ifndef ADMIN_MODE_H_
#define ADMIN_MODE_H_

#include "userData.h"
#include "FSM.h"

state_t AMinputEvHandler(UserData_t * ud);

state_t AMtimerEvHandler(UserData_t * ud);

state_t AMkeycardEvHandler(UserData_t * ud);

#endif /* ADMIN_MODE_H_ */
