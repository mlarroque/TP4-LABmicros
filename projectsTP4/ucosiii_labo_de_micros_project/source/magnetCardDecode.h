/***************************************************************************//**
  @file     magnetCardDecode.h
  @brief
  @author   G5
 ******************************************************************************/

#ifndef _MAGNETCARDDECODE_H_
#define _MAGNETCARDDECODE_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define BITS_CAPACITY 32
#if BITS_CAPACITY >= 32
#define UINT_T uint32_t
#else
#define UINT_T unit8_t
#endif

#define DATA_LEN 256
#define MAX_TRACK_LEN 200

#define TERMINATOR_DATA_DECODED '\0'

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t bufferMagnetDataEncoded_Type;
typedef char bufferMagnetDataDecoded_Type;


_Bool magnetDataParser(bufferMagnetDataEncoded_Type * bufferDataIn, bufferMagnetDataDecoded_Type * bufferDataOut, int * trackFounded);


#endif //_MAGNETCARDDECODE_H_
