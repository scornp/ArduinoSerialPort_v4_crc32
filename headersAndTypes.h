/*
 * headersAndTypes.h
 *
 *  Created on: 16 Mar 2019
 *      Author: Roger
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Arduino.h"

#ifndef HEADERSANDTYPES_H_
#define HEADERSANDTYPES_H_

unsigned char EOT = 0x01;
unsigned char BOT = 0x02;
unsigned char LOK = 0x03;
unsigned char SYNC = 0x04;
unsigned char OK = 0x05;
unsigned char RSD = 0x06;
unsigned char SOK = 0x07;
unsigned char NOK = 0x08;

const int bufSize = 64;
unsigned char oneKbuf[bufSize];

#define DSERIAL Serial

union crcOverlap {
	uint32_t crcInt;
	unsigned char crcArray[4];
};


typedef struct header {
	int32_t fileSize;
	int32_t bufSize;
	//	int32_t numFrames;
	int32_t crcX;
	unsigned char fileName[64];
	int32_t poly;
	int32_t initX;
	uint32_t crcCheck;
} header;


#endif /* HEADERSANDTYPES_H_ */
