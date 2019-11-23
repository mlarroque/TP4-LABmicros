/*******************************************************************************
  @file     cloudRTOS.h
  @brief    Cloud communication using RTOS
  @author   Manuel Mollon
 ******************************************************************************/

#ifndef CLOUDRTOS_H_
#define CLOUDRTOS_H_

/**
 * @brief Initialize cloud communication.
 */
void initCloudRTOS(void);

/**
 * @brief Run communication thread with RTOS.
 */
void runCloudRTOS(void);

#endif /* CLOUDRTOS_H_ */
