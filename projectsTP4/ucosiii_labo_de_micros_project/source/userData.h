/*
 * userData.h
 *
 *  Created on: Sep 6, 2019
 *      Author: Manuel Mollon
 */

#ifndef USERDATA_H_
#define USERDATA_H_

#include "timer.h"
#include "encoder.h"
#include "magnetCardLector.h"

#define ID_LENGTH 8 //Longitud del ID del usuario.
#define PIN_MAX_LENGTH 5 //Longitud maxima del PIN del usuario.
#define PIN_MIN_LENGTH 4 //Longitud minima del PIN del usuario

typedef enum {ADMIN,BASIC,NONE}category_name;

//UserData contiene toda la informacion necesaria que necesita una rutina para manejar
//un evento que se saco de la cola.
typedef struct{
	timerUd_t timerUd;
	magnetLectorData_t magnetLectorUd;
	encoderUd_t encoderUd;
	char received_ID[ID_LENGTH]; //String con el ID recibido del usuario.
	char received_PIN[PIN_MAX_LENGTH]; //String con el PIN recibido del usuario.
	int option; // Si usuario emitio un pedido
	category_name category;
}UserData_t;

#endif /* USERDATA_H_ */
