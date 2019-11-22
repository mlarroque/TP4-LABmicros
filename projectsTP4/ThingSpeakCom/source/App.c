/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "gatewayComm.h"
#include "cloudRTOS.h"
#include "uart.h"
#include "buildingDataBase.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/* SendData: AA 55 C3 3C 07 01 pp pp qq qq rr rr	Tx
 * SendDataOk: AA 55 C3 3C 01 81					Rx
 * SendDataFail: AA 55 C3 3C 01 C1					Rx
 * KeepAlive: AA 55 C3 3C 01 02						Tx
 * KeepAliveOk: AA 55 C3 3C 01 82					Rx
 */

#define CANT	12
#define MSG_CANT	3
static uint16_t msgTx[MSG_CANT] = {0x0003,0x0010,0x000B};
static char msgRx[CANT];
static uint8_t cant;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void App_Init (void)
{
	gatewayCommInit();
	buildingDatabaseInit(3);
}
/* Función que se llama constantemente en un ciclo infinito */

void App_Run (void)
{
	if(uartIsRxMsg(U0)){
		cant = uartReadMsg(U0, msgRx,CANT);
		for(int i = 0; i<cant ;i++){
			gatewayShapeMsg(msgRx[i]);
		}
		if(isGatewayMsgComplete()){
			gatewayDispatchMsg();
			if(isGatewayReadyToSend()){
				gatewaySendMsg(msgTx, MSG_CANT);
			}
			else{
				gatewayActivateResend();
			}
		}
	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 ******************************************************************************/
