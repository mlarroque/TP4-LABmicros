/*
 * encoder.h
 *
 * Header con las definiciones de las funciones necesarias
 * para interpretar lo ingresado por el usuario mediante el encoder.
 *
 *
 *  Created on: Aug 31, 2019
 *      Author: Lu
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdbool.h>

/******************************************************************************
 *									DEFINICIONES
 ******************************************************************************/

typedef enum {UP/*horario*/, DOWN/*antihorario*/, /*BACK*/ ENTER, CANCEL, NO_ENC_EVENT}enc_type;

typedef struct{
	enc_type input;
	_Bool isValid;
}encoderUd_t;

/******************************************************************************
 *							FUNCIONES DEL HEADER
 ******************************************************************************/
//inicializa lo necesario para utilizar el encoder
void initializeEncoder(void);

//popea el primer evento de la cola
encoderUd_t popEncoderEvent(void);

//indica si el primer evento en la cola es válido
_Bool isEncEventValid(void);



#endif /* ENCODER_H_ */
