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
void initCloudRTOS(void * p2resourcesData);

/**
 * @brief Run communication thread with RTOS.
 */
void runCloudRTOS(char * pToBuffer, int bufSize);

#endif /* CLOUDRTOS_H_ */
