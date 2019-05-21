/*
 * textHandling.cpp
 *
 *  Created on: 16 Mar 2019
 *      Author: Roger
 */
//#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void stripCrAndNewLine(char *ptr, int len){
	int i = 0;
	while(1){
		if (*ptr == '\n' || *ptr == '\r'){
			*ptr = '\0';
			break;
		} else {
			ptr++;
			i++;
			if (i == len) {
				*ptr = '\0';
				break;
			}
		}
	}
}

// adjust left and extract cmd string;
char * adjustL(char *ptr, int len){
	while(1){
		if (*ptr == ' '){
			ptr++;
		} else {
			break;
		}
	}
	return ptr;
}
void getArguments(unsigned char *keyBoardInput, int inputBufferSize, int *nargs, unsigned char *argv[10]){

	*nargs = 0;
	for(int i = 0; i < 10; i++) argv[i] = NULL;

	//	printf("<local><getArguments><01> : arg number %d \n", *nargs);

	if (keyBoardInput[0] != ' '){
		argv[0] = &keyBoardInput[0];
		(*nargs)++;
	}
	//	printf("<local><getArguments><02> : arg number %d \n", *nargs);

	for(int i = 0; i < inputBufferSize - 1; i++){
		if (keyBoardInput[i] == ' ' && keyBoardInput[i + 1] != ' '){
			argv[*nargs] = &keyBoardInput[i + 1];
			(*nargs)++;
		}
	}


	for(int i = 0; i < inputBufferSize; i++){
		if (keyBoardInput[i] == ' ' ||
				keyBoardInput[i] == '\r' ||
				keyBoardInput[i] == '\n'){
			keyBoardInput[i] = '\0';
		}
	}

	// set commands to uppercase
	for(unsigned char *j = argv[0]; j < (argv[0] + strlen((char *)argv[0])); j++){
		if (*j >= 'a' && *j <= 'z') *j = *j - 32;
	}

}
