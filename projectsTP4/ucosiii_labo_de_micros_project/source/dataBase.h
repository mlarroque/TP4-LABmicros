/*
 * dataBase.h
 *
 *  Created on: Sep 1, 2019
 *      Author: Manuel Mollon
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <stdbool.h>
#include "userData.h"

#define MAX_CANT_USERS	10

typedef enum {SUCCESSFULL,DATABASE_FULL,ID_ALREADY_EXISTS,ID_NOT_FOUND}status;

typedef struct{
	char usersID[ID_LENGTH];
	char usersPIN[PIN_MAX_LENGTH];
	category_name category;
}user_t;

typedef struct{
	user_t userList[MAX_CANT_USERS];
	int top;
}dataBase_t;

void initializeDataBase(void);

status addUser(user_t newUser);

status removeUser(user_t userToDelete);

status removeUserID(char usersID[ID_LENGTH]);

bool verifyID(char usersID[ID_LENGTH]);

bool verifyPIN(char usersID[ID_LENGTH], char usersPIN[PIN_MAX_LENGTH]);

category_name verifyCategory(char usersID[ID_LENGTH]);

void changePIN(char usersID[ID_LENGTH], char usersNewPIN[PIN_MAX_LENGTH]);

#endif /* DATABASE_H_ */
