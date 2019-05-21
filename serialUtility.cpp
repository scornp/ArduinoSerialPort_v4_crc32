/*
 * serialUtility.cpp
 *
 *  Created on: 17 Mar 2019
 *      Author: Roger
 */

#include "common.h"


void readStream(unsigned char *ptr, int len){
int i = 0;
	while(i < len){
		if (Serial.available() > 0){
			*ptr = (unsigned char)Serial.read();
			ptr++;
			i++;
		}
	}
}

int readStream1(unsigned char *ptr, int len){
int i = 0;
	while(i < len){
		if (Serial.available() > 0){
			*ptr = (unsigned char)Serial.read();
			ptr++;
			i++;
		}
	}
	return i;
}


int getInput(unsigned char *cmdStr){
unsigned char c;
int count;

	count = 0;
	while (DSERIAL.available() > 0) {
		c = DSERIAL.read();
		count++;

		if (c == '\n' || c == '\r') {
			break;
		} else {
			*cmdStr = c;
			cmdStr++;
		}
		*cmdStr ='\0';
		delay(20);
	}
	return count;
}

