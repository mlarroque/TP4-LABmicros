/*******************************************************************************
  @file     cloudRTOS.h
  @brief    Cloud communication using RTOS
  @author   Manuel Mollon
 ******************************************************************************/

#ifndef CLOUDRTOS_H_
#define CLOUDRTOS_H_

/**
 * @brief Initialize cloud communication.
 * @param p2resourcesData Pointer to thread resources.
 */
void initCloudRTOS(void * p2resourcesData);

/**
 * @brief Run communication thread with RTOS.
 * @param pToBufffer Array where event info is stored.
 * @param bufSize Size of data array.
 */
void runCloudRTOS(char * pToBuffer, int bufSize);

#endif /* CLOUDRTOS_H_ */
