/*******************************************************************************
  @file     connectivityLED.h
  @brief    LED Connection Manager
  @author   Manuel Mollon
 ******************************************************************************/

#ifndef CONNECTIVITYLED_H_
#define CONNECTIVITYLED_H_

#include <stdbool.h>

/**
 * @brief Initializes Module.
 */
void connectivityLedInit(void);

/**
 * @brief Switches LED connectivity ON or OFF.
 * @param connected True to switch LED green. False to switch LED to red.
 */
void updateConnectivityLed(bool connected);

#endif /* CONNECTIVITYLED_H_ */
