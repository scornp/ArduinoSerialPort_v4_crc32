/*
 * crcStuff.h
 *
 *  Created on: 16 Mar 2019
 *      Author: Roger
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef CRCSTUFF_H_
#define CRCSTUFF_H_

// add in the crc material
const int32_t order = 32;
const uint32_t polynom = 0x4c11db7;
const int32_t direct = 1;
const uint32_t crcinit = 0xffffffff;
const uint32_t crcxor = 0xffffffff;
const int32_t refin = 1;
const int32_t refout = 1;



#endif /* CRCSTUFF_H_ */
