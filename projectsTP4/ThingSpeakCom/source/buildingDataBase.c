/*
 * buildingDataBase.c
 *
 *  Created on: Nov 20, 2019
 *      Author: Manuel Mollon
 */

#include "buildingDataBase.h"

#define MAX_FLOORS	100

static uint16_t	floor[MAX_FLOORS];
static uint8_t activeFloors = 0;
static bool initialized = false;

/*******************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 ******************************************************************************/

void buildingDatabaseInit(uint8_t floorsNbr){
	if(initialized || (floorsNbr == 0)){
		return;
	}
	activeFloors = floorsNbr;
	for(int i = 0; i<activeFloors ;i++){
		floor[i] = 0; /* Initialize Empty Floor. */
	}
	initialized = true;
}

void updateBuildingDatabase(uint8_t floorToUpdate, bool entering){
	if( (floorToUpdate > 0) && (floorToUpdate <= activeFloors)){ /* Parse floor. */
		if(entering){ /* Entering Building. */
			floor[floorToUpdate-1]++;
		}
		else{ /* Exiting Building. */
			floor[floorToUpdate-1]--;
		}
	}
	return;
}

uint8_t getBuildingDatabase(uint16_t * data, uint8_t cant){
	uint8_t maxIdx = (cant<activeFloors)?cant:activeFloors; /* Select limiting variable. */
	for(int i = 0;i<maxIdx;i++){
		data[i]=floor[i];
	}
	return maxIdx;
}


