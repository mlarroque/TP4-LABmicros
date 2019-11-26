/*
 * cloudRTOS.c
 *
 *  Created on: Nov 15, 2019
 *      Author: Manuel Mollon
 */

#ifndef CLOUDRTOS_C_
#define CLOUDRTOS_C_

#include "cloudRTOS.h"
#include "gatewayComm.h"
#include "buildingDataBase.h"
#include <stdbool.h>
#include <stdint.h>



#define	RX_EV	0x00
#define	UPDATE_EV	0x01
#define NAME_INDEX	0
#define DATA_INDEX	1
#define DATA_MAX	20
#define DATABASE_SIZE	3

//static char * pToBuffer;
static uint8_t	cant;
uint16_t data[DATA_MAX];

void initCloudRTOS(void * p2resourcesData){
	buildingDatabaseInit(DATABASE_SIZE);
	gatewayCommInit(p2resourcesData);

}

void runCloudRTOS(char * pToBuffer, int bufSize){
	if(pToBuffer != 0)
	{
		//OS_ERR os_err;
		//OS_ERR os_errAUX;
		//OS_MSG_SIZE msgSize;
		//FOREVER{ /* Infinite loop. */
			//pToBuffer = OSQPend((OS_Q *) p2dataInterface, 0, &msgSize, 0, &os_err);
			int8_t dataByte = pToBuffer[DATA_INDEX];
			switch(pToBuffer[NAME_INDEX]){
				case RX_EV: /* Message received from cloud. */
					gatewayShapeMsg(pToBuffer[DATA_INDEX]);
					if(isGatewayMsgComplete()){ /* Message Completed.*/
						gatewayDispatchMsg();
					}
					break;
				case UPDATE_EV: /* New info for database. */
					if(dataByte>0){
						updateBuildingDatabase((uint8_t)dataByte,true);
					}
					else{
						dataByte = -dataByte;
						updateBuildingDatabase((uint8_t)dataByte,false);
					}
					if(isGatewayReadyToSend()){
						cant = getBuildingDatabase(data,DATA_MAX);
						gatewaySendMsg(data,cant);
					}
					else{
						/* Ask gateway to re send message after time out. */
						gatewayActivateResend();
					}
					break;
				}
			}
	//}

}

#endif /* CLOUDRTOS_C_ */
