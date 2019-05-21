/*
 * textHandling.h
 *
 *  Created on: 16 Mar 2019
 *      Author: Roger
 */

#ifndef TEXTHANDLING_H_
#define TEXTHANDLING_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void getArguments(unsigned char *, int, int *, unsigned char **);
char * adjustL(char *, int);
void stripCrAndNewLine(char *, int);


#endif /* TEXTHANDLING_H_ */
