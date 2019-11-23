/*
 * dataBase.c
 *
 *  Created on: Sep 1, 2019
 *      Author: Manuel Mollon
 */

#include "dataBase.h"
#include <stdbool.h>

static dataBase_t dataBase;

void initializeDataBase(void)
{
	dataBase.top = -1;
	user_t newUser1 = {{'6','0','6','1','2','6','8','3'},{'0','0','0','0','0'},ADMIN};
	user_t newUser2 = {{'6','0','6','1','2','6','8','4'},{'0','0','0','0','1'},BASIC};
	user_t newUser3 = {{'4','5','1','7','6','6','0','1'},{'0','0','0','0',' '},ADMIN};
	addUser(newUser1);
	addUser(newUser2);
	addUser(newUser3);

}

void changePIN(char usersID[ID_LENGTH], char usersNewPIN[PIN_MAX_LENGTH])
{
	// checks if ID is on list
	int i,j;
	for(i=0 ; i< (dataBase.top + 1) ; i++){
		bool same = true;
		for(j=0 ; j<ID_LENGTH ; j++){
			if(dataBase.userList[i].usersID[j] != usersID[j]){
				same = false;
			}
		}
		if(same){
			break;
		}
	}
	for(j=0;j<PIN_MAX_LENGTH;j++){
		dataBase.userList[i].usersPIN[j] = usersNewPIN[j];
	}
}

status addUser(user_t newUser)
{
	if(dataBase.top == MAX_CANT_USERS-1){ // user overflow
		return DATABASE_FULL;
	}
	else{
		// checks if ID already on list
		bool IDused = verifyID(newUser.usersID);
		if(!IDused)
		{
			// if not on list, user is added
			dataBase.top += 1;
			dataBase.userList[dataBase.top] = newUser;
			return SUCCESSFULL;
		}
		else
		{
			return ID_ALREADY_EXISTS;
		}
	}
}

status removeUser(user_t userToDelete)
{
		// checks if ID is on list
		bool IDfound = false;
		int i,j; //position where ID is, if found
		for(i=0 ; i< (dataBase.top + 1) ; ++i){
			bool same = true;
			for(j=0 ; i<ID_LENGTH ; ++i){
				if(dataBase.userList[i].usersID[j] != userToDelete.usersID[j]){
					same = false;
				}
			}
			if(same){
				IDfound = true;
				break;
			}
		}
		if(IDfound)
		{
			// if on list, user is deleted
			user_t topUser = dataBase.userList[dataBase.top];
			dataBase.userList[i] = topUser; // overwrites user to be removed
			dataBase.top -= 1; // decrements top pointer
			return SUCCESSFULL;
		}
		else
		{
			return ID_NOT_FOUND;
		}
}

status removeUserID(char usersID[ID_LENGTH])
{
	// checks if ID is on list
	bool IDfound = false;
	int i,j; //position where ID is, if found
	for(i=0 ; i< (dataBase.top + 1) ; ++i){
		bool same = true;
		for(j=0 ; j<ID_LENGTH ; j++){
			if(dataBase.userList[i].usersID[j] != usersID[j]){
				same = false;
			}
		}
		if(same){
			IDfound = true;
			break;
		}
	}
	if(IDfound)
	{
		// if on list, user is deleted
		user_t topUser = dataBase.userList[dataBase.top];
		dataBase.userList[i] = topUser; // overwrites user to be removed
		dataBase.top -= 1; // decrements top pointer
		return SUCCESSFULL;
	}
	else
	{
		return ID_NOT_FOUND;
	}
}

bool verifyID(char usersID[ID_LENGTH])
{
	// checks if ID is on list
	bool IDfound = false;
	int i,j;
	for(i=0 ; i< (dataBase.top + 1) ; i++){
		bool same = true;
		for(j=0 ; j<ID_LENGTH ; j++){
			if(dataBase.userList[i].usersID[j] != usersID[j]){
				same = false;
				break;
			}
		}
		if(same){
			IDfound = true;
			break;
		}
	}
	return IDfound;
}

bool verifyPIN(char usersID[ID_LENGTH], char usersPIN[PIN_MAX_LENGTH])
{
	// checks if ID is on list
	int i,j;
	for(i=0 ; i< (dataBase.top + 1) ; i++){
		bool same = true;
		for(j=0 ; j<ID_LENGTH ; j++){
			if(dataBase.userList[i].usersID[j] != usersID[j]){
				same = false;
				break;
			}
		}
		if(same){
			break;
		}
	}
	bool correctPIN = true;
	for(j=0 ; j<PIN_MAX_LENGTH ; j++){
		if(dataBase.userList[i].usersPIN[j] != usersPIN[j]){
			correctPIN = false;
			break;
		}
	}
	return correctPIN;
}

category_name verifyCategory(char usersID[ID_LENGTH])
{
	// checks if ID is on list
	int i,j;
	for(i=0 ; i< (dataBase.top + 1) ; ++i){
		bool same = true;
		for(j=0 ; j<ID_LENGTH ; ++j){
			if(dataBase.userList[i].usersID[j] != usersID[j]){
				same = false;
				break;
			}
		}
		if(same){
			break;
		}
	}
	return dataBase.userList[i].category;
}
