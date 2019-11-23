/*
 * gatewayComm.c
 *
 *  Created on: Nov 20, 2019
 *      Author: Manuel Mollon
 */

#include "gatewayComm.h"
#include "connectivityLED.h"
#include "buildingDataBase.h"
#include "PIT.h"
#include "uart.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define BUFFER_SIZE	50
#define DATA_SIZE	5
#define TX_PREFIX_CANT	6
/* Timer values. */
#define CONNECTIVITY_TIMER_VAL	5000000UL
#define CONN_TIMEOUT_TIMER_VAL	3000000UL
#define MESS_TIMEOUT_TIMER_VAL	16000000UL
#define TIMEOUT_TIMER_VAL	5000000UL
/* Message Codes. */
#define KEEP_ALIVE_CANT	6
#define CODE_IDX	5
#define KEEP_ALIVE_OK	0x82
#define SEND_DATA_OK	0x81
#define SEND_DATA_FAIL	0xC1
/* PIT timers. */
#define CONNECTIVITY_PIT	0
#define CONN_TIMEOUT_PIT	1
#define MESS_TIMEOUT_PIT	2
#define TIMEOUT_PIT			2

/*******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************/

static bool resend = false;
static bool readyToSend = false;
static bool readyToSendKA = false;
static bool connected = false;
static bool keepAlive = false;
static bool msgCompleted = false;
static char msgTx[BUFFER_SIZE] = {0xAA,0x55,0xC3,0x3C,0x07,0x01};
static const char keepAliveTx[] = {0xAA,0x55,0xC3,0x3C,0x01,0x02};
static char msgRx;
static uint8_t msgIdx = 0;
static const char msgPrefix[] = {0xAA,0x55,0xC3,0x3C,0x01};

/*******************************************************************************
 * LOCAL FUNCTION DECLARATIONS
 ******************************************************************************/

/**
 * @brief Shapes data into Tx message buffer.
 * @param data Bytes to shape.
 * @param cant Number of bytes the shape.
 */
void gatewayShapeData(uint16_t * data, uint8_t cant);

/**
 * @brief Timer connectivity callback to send KeepAlive.
 */
void connectivityCallback(void);

/**
 * @brief Timer connectivity callback after waiting for KeepAlive.
 */
void connectivityTimeoutCallback(void);

/**
 * @brief Timer callback to wait until next send Message instruction.
 */
void msgTimeoutCallback(void);

/**
 * @brief Timer callback when no response from Gateway.
 */
void timeoutCallback(void);

/*******************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 ******************************************************************************/

void gatewayCommInit(void * p2resourcesData){
	uart_cfg_t UARTconfig = {
		1200, 					/* Baudrate. */
		NON_BLOCKING_SIMPLE, 	/* Mode. */
		NO_PARITY_UART, 		/* Parity. */
		NBITS_8, 				/* Bits. */
		0,
		0,
		p2resourcesData
	};
	pit_config_t PITconfig = {
		CONNECTIVITY_TIMER_VAL, /* Value of timer. In micro seconds if not in chain mode. In cycles if in chain mode. */
		CONNECTIVITY_PIT, 		/* Number of PIT timer. */
		false, 					/* True if timer in Chain Mode. */
		connectivityCallback 	/* Callback for interrupt. NULL if interrupt is disabled. */
	};
	/* Init Modules. */
	PITinit();
	uartInit(U0, UARTconfig);
	connectivityLedInit();
	buildingDatabaseInit(0);
	/* Connectivity timer init. */
	PITstartTimer(&PITconfig);
	connected = true;
	readyToSend = true;
	readyToSendKA = true;
	updateConnectivityLed(connected);
}

void gatewayShapeMsg(char newChar){
	if(msgIdx<CODE_IDX){
		if(msgPrefix[msgIdx] == newChar){ /* Check prefix of message. (Same for every message) */
			msgIdx++;
		}
		else{
			msgIdx = 0; /* Invalid Message, start looking again.*/
		}
	}
	else if(msgIdx == CODE_IDX){
		msgRx = newChar;
		msgCompleted = true;
	}
}

void gatewayDispatchMsg(void){
	if(!msgCompleted){
		return;
	}
	pit_config_t PITconfig = {
		MESS_TIMEOUT_TIMER_VAL, /* Value of timer. In micro seconds if not in chain mode. In cycles if in chain mode. */
		MESS_TIMEOUT_PIT, 		/* Number of PIT timer. */
		false, 					/* True if timer in Chain Mode. */
		msgTimeoutCallback 		/* Callback for interrupt. NULL if interrupt is disabled. */
	};
	switch(msgRx){
	case KEEP_ALIVE_OK:
		/* Connectivity is up. */
		keepAlive = true;
		break;
	case SEND_DATA_OK:
		/* Data was send, so we start timer. */
		readyToSendKA = true;
		PITstopTimer(TIMEOUT_PIT);
		PITstartTimer(&PITconfig);
		break;
	case SEND_DATA_FAIL:
		resend = true;
		readyToSendKA = true;
		break;
	}
	msgIdx = 0;
	msgCompleted = false;
}

bool isGatewayMsgComplete(void){
	return msgCompleted;
}

bool isGatewayReadyToSend(void){
	if(connected && readyToSend){
		return true;
	}
	else{
		return false;
	}
}

void gatewaySendMsg(uint16_t * data, uint8_t cant){
	if(!readyToSend){
		return;
	}
	/* Create Message. */
	gatewayShapeData(data,cant);
	/* Send message. */
	uartWriteMsg(U0, msgTx, TX_PREFIX_CANT+(cant*2));
	readyToSend = false;
	readyToSendKA = false;

	/* Start Timeout timer. Timer stops if module receives SendDataOk or SendDataFail. */
	pit_config_t PITconfig = {
		TIMEOUT_TIMER_VAL, /* Value of timer. In micro seconds if not in chain mode. In cycles if in chain mode. */
		TIMEOUT_PIT, 		/* Number of PIT timer. */
		false, 					/* True if timer in Chain Mode. */
		timeoutCallback 		/* Callback for interrupt. NULL if interrupt is disabled. */
	};
	PITstartTimer(&PITconfig);
}

void gatewayActivateResend(void){
	/* Set flag to re send after timeout. */
	resend = true;
}

/*******************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 ******************************************************************************/

void gatewayShapeData(uint16_t * data, uint8_t cant){
	uint8_t maxIdx = ((2*cant)<BUFFER_SIZE)?(2*cant):BUFFER_SIZE; /* Select limiting variable. */
	int j = 0;
	for(int i = 0;i<maxIdx;i++,j++){
		uint8_t LSByte = ((uint8_t)data[j] >> (0 << 3)) & 0xff;
		msgTx[CODE_IDX+1+i] = LSByte;
		i++;
		uint8_t MSByte = ((uint8_t)data[j] >> (1 << 3)) & 0xff;
		msgTx[CODE_IDX+1+i] = MSByte;
	}
}

void connectivityCallback(void){
	if(readyToSendKA){
		keepAlive = false;
		/* Start Timeout. */
		pit_config_t PITconfig = {
			CONN_TIMEOUT_TIMER_VAL, 		/* Value of timer. In micro seconds if not in chain mode. In cycles if in chain mode. */
			CONN_TIMEOUT_PIT, 				/* Number of PIT timer. */
			false, 							/* True if timer in Chain Mode. */
			connectivityTimeoutCallback 	/* Callback for interrupt. NULL if interrupt is disabled. */
		};
		uartWriteMsg(U0, keepAliveTx, KEEP_ALIVE_CANT);
		PITstartTimer(&PITconfig);
	}
	else{
		/* Busy, send next time. */
	}
}

void connectivityTimeoutCallback(void){
	if(keepAlive){
		if(connected){ /* Previously Connected in last keepAlive check. */
			/* Do nothing. */
		}
		else{
			connected = true;
			updateConnectivityLed(connected);
			/* Send new message for new connection. */
			if(readyToSend){
				/* Send new data. */
				uint16_t data[DATA_SIZE];
				uint8_t cant = getBuildingDatabase(data,DATA_SIZE);
				gatewaySendMsg(data, cant);
			}
			else{
				resend = true;
			}
		}
	}
	else{
		if(connected == false){
			/* Still no connection. */
		}
		else{
			connected = false;
			updateConnectivityLed(connected); /* Update LED to not connected. */
		}
	}
	/* Stop Timer. */
	PITstopTimer(CONN_TIMEOUT_PIT);
}

void msgTimeoutCallback(void){
	if(resend){
		/* Send new data. */
		uint16_t data[DATA_SIZE];
		uint8_t cant = getBuildingDatabase(data,DATA_SIZE);
		/* Create Message. */
		gatewayShapeData(data,cant);
		/* Send message. */
		uartWriteMsg(U0, msgTx, TX_PREFIX_CANT+(cant*2));
		resend = false;
		readyToSendKA = false;
	}
	else{
		readyToSend = true;
		/* Stop Timer. */
		PITstopTimer(MESS_TIMEOUT_PIT);
	}
}

void timeoutCallback(void){
	PITstopTimer(TIMEOUT_PIT);
	connected = false;
	updateConnectivityLed(connected);
	readyToSendKA = true;
	readyToSend = true;
	resend = true;
}
