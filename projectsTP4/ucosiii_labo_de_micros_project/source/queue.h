/*
 * queue.h
 *
 *  Created on: Sep 1, 2019
 *      Author: Manuel Mollon
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define MAX_EVENTS 30

#include "userData.h"


typedef enum {INPUT_EV, TIMER_EV, KEYCARD_EV, NUM_EVENTS}ev_name;

ev_name getEvent(UserData_t * ud);

void userDataReset(bool resetID, bool resetPIN, bool resetCategory, bool resetOption, UserData_t * ud);

#endif /* QUEUE_H_ */
