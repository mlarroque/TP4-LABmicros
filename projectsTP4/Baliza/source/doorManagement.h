/*
 * doorManagement.h
 *
 *  Created on: 4 sep. 2019
 *      Author: G5
 */

#ifndef DOORMANAGEMENT_H_
#define DOORMANAGEMENT_H_

#define TIME_OUT_DOOR 5000

void doorInit(void);
void openDoor(void);
void closeDoor(void);
void toggleDoor(void);
void openDoorTemporally(void);

#endif /* DOORMANAGEMENT_H_ */
