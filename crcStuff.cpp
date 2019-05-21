/*
 * crcStuff.cpp
 *
 *  Created on: 16 Mar 2019
 *      Author: Roger
 */

#include "crcStuff.h"

uint32_t crcinit_direct;
uint32_t crcmask;
uint32_t crchighbit;
uint32_t crcinit_nondirect;

uint32_t reflect (uint32_t crc, int32_t bitnum) {

	// reflects the lower 'bitnum' bits of 'crc'

	uint32_t i, j=1, crcout=0;

	for (i=(uint32_t)1<<(bitnum-1); i; i>>=1) {
		if (crc & i) crcout|=j;
		j<<= 1;
	}
	return (crcout);
}

uint32_t crcbitbybitfast(unsigned char* p, uint32_t len) {

	// fast bit by bit algorithm without augmented zero bytes.
	// does not use lookup table, suited for polynom orders between 1...32.

	uint32_t i, j, c, bit;
	uint32_t crc = crcinit_direct;

	//	printf("crcbitbybitfast crc 01 check sum is %d \n", crc);

	for (i=0; i<len; i++) {
		c = (uint32_t)*p++;
		//	printf("crc is %d\n", crc);

		if (refin) c = reflect(c, 8);

		for (j=0x80; j; j>>=1) {

			bit = crc & crchighbit;
			crc<<= 1;
			if (c & j) bit^= crchighbit;
			if (bit) crc^= polynom;
		}
	}

	if (refout) crc=reflect(crc, order);
	crc^= crcxor;
	crc&= crcmask;

	return(crc);
}

uint32_t checkMaskEtc(){
	crcmask = ((((uint32_t )1 << (order - 1)) - 1) << 1) | 1;
	crchighbit = (uint32_t )1 << (order-1);

	return(0);
}

