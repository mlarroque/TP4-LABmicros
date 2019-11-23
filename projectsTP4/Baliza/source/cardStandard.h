/***************************************************************************//**
  @file     cardStandard.h
  @brief    
  @author   
 ******************************************************************************/

#ifndef _CARDSTANDARD_H_
#define _CARDSTANDARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


#define NUMBER_OF_TRACKS_AVAILABLE 3

#define PAR 0
#define IMPAR 1
#define PARITY_TRACK1 IMPAR
#define PARITY_TRACK2 IMPAR
#define PARITY_TRACK3 IMPAR

#define DATA_OFFSET_ASCII_TRACK1 32
#define DATA_OFFSET_ASCII_TRACK2 48
#define DATA_OFFSET_ASCII_TRACK3 DATA_OFFSET_ASCII_TRACK2

#define NUMBER_OF_CHARS_TRACK1 79
#define NUMBER_OF_CHARS_TRACK2 40
#define NUMBER_OF_CHARS_TRACK3 107

#define SIZE_OF_CHARS_TRACK1 7
#define SIZE_OF_CHARS_TRACK2 5
#define SIZE_OF_CHARS_TRACK3 5

#define NUMBER_OF_BITS_TRACK1 (SIZE_OF_CHARS_TRACK1 * NUMBER_OF_CHARS_TRACK1)
#define NUMBER_OF_BITS_TRACK2 (SIZE_OF_CHARS_TRACK2 * NUMBER_OF_CHARS_TRACK2)
#define NUMBER_OF_BITS_TRACK3 (SIZE_OF_CHARS_TRACK3 * NUMBER_OF_CHARS_TRACK3)

#define START_SENTINEL_TRACK1 '%' - DATA_OFFSET_ASCII_TRACK1 //000101
#define FIELD_SEPARATOR_TRACK1 '^' - DATA_OFFSET_ASCII_TRACK1 //111110
#define END_SENTINEL_TRACK1 '?' - DATA_OFFSET_ASCII_TRACK1  //011111
#define MIN_VALUE_TRACK1 '0' - DATA_OFFSET_ASCII_TRACK1
#define MAX_VALUE_TRACK1 'Z' - DATA_OFFSET_ASCII_TRACK1
#define N_OF_FIELDS_SEPARATORS_TRACK1 2
#define N_OF_FORMAT_CODE_TRACK1 1


#define START_SENTINEL_TRACK2 ';' - DATA_OFFSET_ASCII_TRACK2  //1011
#define FIELD_SEPARATOR_TRACK2 '=' - DATA_OFFSET_ASCII_TRACK2 //1101
#define END_SENTINEL_TRACK2 '?' - DATA_OFFSET_ASCII_TRACK2    //1111
#define MIN_VALUE_TRACK2 '0' - DATA_OFFSET_ASCII_TRACK2
#define MAX_VALUE_TRACK2 '9' - DATA_OFFSET_ASCII_TRACK2
#define N_OF_FIELDS_SEPARATORS_TRACK2 1
#define N_OF_FORMAT_CODE_TRACK2 0

#define START_SENTINEL_TRACK3 START_SENTINEL_TRACK2
#define FIELD_SEPARATOR_TRACK3 FIELD_SEPARATOR_TRACK2
#define END_SENTINEL_TRACK3 END_SENTINEL_TRACK2 
#define MIN_VALUE_TRACK3 MIN_VALUE_TRACK2
#define MAX_VALUE_TRACK3 MAX_VALUE_TRACK2
#define N_OF_FIELDS_SEPARATORS_TRACK3 N_OF_FIELDS_SEPARATORS_TRACK2
#define N_OF_FORMAT_CODE_TRACK3 2

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/





#endif //_CARDSTANDARD_H_
