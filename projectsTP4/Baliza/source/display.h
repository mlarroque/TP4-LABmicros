/*
 * display.h
 *
 *  Created on: Aug 28, 2019
 *      Author: G5
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <stdbool.h>
#include <stdint.h>

/*********************************************************************************************************
 * 										DEFINCIONES Y TIPOS DE DATOS
 ********************************************************************************************************/
#define DISPLAY_SIZE 4

/**********************************************************************************************************
 * 										DECLARACION DE FUNCIONES
 **********************************************************************************************************/

//Inicializa los recursos necesarios para utilizar el display
void InitializeDisplay(void);

//Borra el contenido del display
void ClearDisplay(void);

//Imprime el string que recibe en el display.El segundo argumento indica si solo se desean mostrar
//los ultimos simbolos del string tal que el mensaje entre en el display(moving_string=false),
//o si se desea que el mensaje vaya apareciendo en el display de derecha a izquierda(moving_string=true).
void PrintMessage(const char* string, bool moving_string);

//Cambia la luminosidad del display, recibe un numero de 1 a 4,
//donde 1 simboliza 25% de intensidad y 4 simboliza 100%.
void SetBrightness(unsigned char brightness_factor);

//Actualiza el contenido del display.
void UpdateDisplay(void);

#endif /* DISPLAY_H_ */



