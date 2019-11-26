/*******************************************************************************
  @file     buildingDataBase.h
  @brief    Data Base that manages traffic in building
  @author   Manuel Mollon
 ******************************************************************************/

#ifndef BUILDINGDATABASE_H_
#define BUILDINGDATABASE_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize Database.
 * @param uint_8 floor Number of floors that the database will have.
 */
void buildingDatabaseInit(uint8_t floors);

/**
 * @brief Updates one floor in the database.
 * @param floorToUpdate Floor that will be updated.
 * @param entering True if the person is entering the floor. False if the person is leaving.
 */
void updateBuildingDatabase(uint8_t floorToUpdate, bool entering);

/**
 * @brief Enable interrupt for running timer.
 * @param data Where the data will be stored.
 * @param cant Max quantity data allows.
 * @return Number of bytes returned in data.
 */
uint8_t getBuildingDatabase(uint16_t * data, uint8_t cant);


/**
 * @brief check if 'idFloor' indicates a valid floor and return the corresponding floor number.
 * @param idFloor indicates an id floor.
 * @return number of the floor in case idFloor is valid, if not return 0.
 */
uint8_t checkFloor(char idFloor);

#endif /* BUILDINGDATABASE_H_ */
