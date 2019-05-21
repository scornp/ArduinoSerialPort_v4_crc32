/*
 * common.h
 *
 *  Created on: 17 Mar 2019
 *      Author: Roger
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <SPI.h>
#include <SdFat.h>


void getArguments(unsigned char *, int, int *, unsigned char **);
char *adjustL(char *, int);
void stripCrAndNewLine(char *, int);
void readStream(unsigned char *, int);
int readStream1(unsigned char *, int);
int getInput(unsigned char *);
void recvFile(unsigned char *, int);
void sendFile(SdFat *, int *, unsigned char **, unsigned char *, int);

#define DSERIAL Serial

extern const int bufs;
extern unsigned char myArray[];


uint32_t reflect (uint32_t, int32_t);

uint32_t crcbitbybitfast(unsigned char*, uint32_t);

uint32_t checkMaskEtc();




#ifndef COMMON_H_
#define COMMON_H_


typedef struct header {
	int32_t fileSize;
	int32_t bufSize;
	int32_t crcX;
	unsigned char fileName[64];
	int32_t poly;
	int32_t initX;
	uint32_t crcCheck;
} header;


union crcOverlap {
	uint32_t crcInt;
	unsigned char crcArray[4];
};


const unsigned char EOT = 0x01;
const unsigned char BOT = 0x02;
const unsigned char LOK = 0x03;
const unsigned char SYNC = 0x04;
const unsigned char OK = 0x05;
const unsigned char RSD = 0x06;
const unsigned char SOK = 0x07;
const unsigned char NOK = 0x08;




#endif /* COMMON_H_ */
