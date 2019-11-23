/*
 * display.c
 *
 *  Created on: Aug 31, 2019
 *      Author: G5
 */
#include "display.h"
#include "SegmentDisplay.h"
#include "timer.h"
#include "timer_queue.h"

#define STRING_TIME 325 //Delay en ms entre cada shifteo hacia la izquierda.
#define FPS 60 //Frames per second
#define MIN_BRIGHTNESS 1
#define MAX_BRIGHTNESS 4
#define MS_BETWEEN_SYMBOLS ( (1000/FPS)/(DISPLAY_SIZE) )
/******************************************************************************
 *
 * 							VARIABLES GLOBALES
 *
 ******************************************************************************/
static const char* current_string;
static int string_position;
static int display_position;
static int string_size;
static unsigned int brigthness;
static unsigned char display_counter; //cuantos llamados faltan para pasar al proximo simbolo
static bool initialized = false;

/*******************************************************************************
 *
 * 							FUNCIONES LOCALES
 *
 *******************************************************************************/
unsigned int GetStringSize(const char* str);
void GenerateDisplayEv(void);
void ShiftLeft(void); //desplaza todos los caracteres una posicion hacia la izquierda.

/******************************************************************************
 *
 *							FUNCIONES DEL HEADER
 *
 ******************************************************************************/

void InitializeDisplay(void)
{
	if(!initialized)
	{
		InitializeSegmentDisplay();
		InitializeTimers();
		InitializeTimerQueue();
		ClearDisplay();
		SetTimer(DISPLAY, MS_BETWEEN_SYMBOLS/MAX_BRIGHTNESS, &GenerateDisplayEv);
		SetTimer(MESSAGE,STRING_TIME, &ShiftLeft);//Setteo el timer con la velocidad de movimiento del string.
		DisableTimer(MESSAGE); //Por default asumo que se desea un mensaje que nose mueva a traves del display.
		brigthness = MAX_BRIGHTNESS; //Por default comienza con la intensidad del display al maximo.
		initialized = true;
		display_counter= 0;
	}
}
void ClearDisplay(void)
{
	current_string = "";
	string_position = 0;
	display_position = 0;
	string_size = -1;
}
void PrintMessage(const char* string, bool moving_string)

{
	ClearDisplay();
	string_size = INT_FAST8_MAX; //Asumo que el string es de largo 'infinito'.

	if(!moving_string) //Solo muestro los ultimos caracteres que entran
	{
		DisableTimer(MESSAGE);//Deshabilito el timer
		string_size = GetStringSize(string);
		if(string_size > DISPLAY_SIZE)
		{
			current_string = string +(string_size - DISPLAY_SIZE);
		}
		else
		{
			current_string = string;
		}
		string_position = 0;
		display_position = 0;

	}
	else
	{
		current_string = string;
		string_position = 0;
		display_position = DISPLAY_SIZE-1; //El mensaje se mueve de derecha a izquierda.
		EnableTimer(MESSAGE);
	}
}

void ShiftLeft(void)
{
	string_position++;
}

void SetBrightness(unsigned char brightness_factor)
{
	if( (brightness_factor >= MIN_BRIGHTNESS) && (brightness_factor <= MAX_BRIGHTNESS) )
	{
		brigthness = brightness_factor;

	}
}

void UpdateDisplay(void)
{
	++display_counter;
	if(display_counter == MAX_BRIGHTNESS)
	{
		display_counter = 0; //Paso al proximo simbolo y reinicio el contador
		string_position++;
		display_position++;
		if(display_position == DISPLAY_SIZE)
		{
			string_position -= DISPLAY_SIZE;
			display_position -= DISPLAY_SIZE;
		}
		if(string_position < 0)
		{
			PrintChar(' ',display_position); //Imprimo espacio en blanco
		}
		else
		{
			if(string_position > string_size)
			{
				PrintChar(' ',display_position);
				if(string_position == (string_size + DISPLAY_SIZE) ) //Si se mostro todo el mensaje, vuelve a pasarlo.
				{
					string_position = 0;
					display_position = DISPLAY_SIZE-1;
				}
			}
			else if(current_string[string_position] == '\0')
			{
				string_size = string_position;
				PrintChar(' ',display_position);
			}
			else
			{
				PrintChar(current_string[string_position],display_position);
			}
		}

	}
	else if( display_counter>= brigthness )
	{
		PrintChar(' ',display_position); //Imprimo espacio en blanco
		return;
	}

}

/*******************************************************************************
 *
 * 							FUNCIONES LOCALES
 *
 *******************************************************************************/

unsigned int GetStringSize(const char* str)
{
	unsigned int size = 0;
	while (str[size++] != '\0');
	return --size;
}

void GenerateDisplayEv(void)
{
	PushTimerEv(DISPLAY);
}



