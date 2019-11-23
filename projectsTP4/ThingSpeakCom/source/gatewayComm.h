/*******************************************************************************
  @file     gatewayComm.h
  @brief    Handles UART communication with gateway.js
  @author   Manuel Mollon
 ******************************************************************************/

#ifndef GATEWAYCOMM_H_
#define GATEWAYCOMM_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes Gateway Communication.
 */
void gatewayCommInit(void);

/**
 * @brief Adds new char to received message buffer.
 * @param newChar New byte received.
 */
void gatewayShapeMsg(char newChar);

/**
 * @brief Takes care of the received message. Always check if it was completed before
 */
void gatewayDispatchMsg(void);

/**
 * @brief Checks if buffer for received message is completed.
 * @return True if Message is completed.
 */
bool isGatewayMsgComplete(void);

/**
 * @brief Checks if driver is ready to send new message via UART.
 * @return True if driver is ready to send message.
 */
bool isGatewayReadyToSend(void);

/**
 * @brief Sends new data as message via UART.
 * @param data Bytes to send.
 * @param cant Number of bytes to send.
 */
void gatewaySendMsg(uint16_t * data, uint8_t cant);

/**
 * @brief Activates re send option, to send new data when gateway is ready.
 */
void gatewayActivateResend(void);

#endif /* GATEWAYCOMM_H_ */
