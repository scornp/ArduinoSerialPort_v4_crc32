/*
 * sendFile.cpp
 *
 *  Created on: 17 Mar 2019
 *      Author: Roger
 */



#include "common.h"


#define DSERIAL Serial
#define RETRYCOUNT 2

void sendFile(SdFat *sd, int *nargs, unsigned char **argv, unsigned char *oneKbuf, int bufSize){
	SdFile fin;
	//SdFat sd;

	header send;
	union crcOverlap crcSntData;
	union crcOverlap crcClcData;

	unsigned char arduinoFileToSend[20];
	unsigned char hostFileToSaveAs[20];

	if (*nargs == 1){
		strcpy(arduinoFileToSend, "dummyFile");
		strcpy(hostFileToSaveAs, "dummyFile");
	} else if (*nargs == 2) {
		strcpy(arduinoFileToSend, argv[1]);
		strcpy(hostFileToSaveAs, argv[1]);
	} else {
		strcpy(arduinoFileToSend, argv[1]);
		strcpy(hostFileToSaveAs, argv[2]);
	};

	fin.open(sd, arduinoFileToSend, O_READ);

	unsigned char ch;
	unsigned char tmpCrc;

	// block until BOT received
	DSERIAL.write(BOT);
	while(1) {
		if (DSERIAL.available() > 0){
			if (DSERIAL.read() == BOT)
				break;
		}
	}

	//	int bufSize = 1024;
	int32_t numFrames;
	int remainder;
	int32_t fileSize = fin.fileSize(); // 3738
	int crcSize = 4;
	uint32_t crc;
	numFrames = fileSize/(bufSize - crcSize);
	remainder = fileSize % (bufSize - crcSize);

	unsigned char *ptr;

	//create header

	send.fileSize = fin.fileSize();
	send.bufSize = bufSize;
	send.crcX = 1111;
	strcpy(send.fileName, hostFileToSaveAs);
	send.poly = 42;
	send.initX = 25;
	send.crcCheck = 1234;

	//header send;
	// here is where we disassemble the serial stream
	//	unsigned char *ptrs;

	int32_t error = checkMaskEtc();

	ptr = (unsigned char*)(&send);

	for(int32_t i = 0; i < sizeof(header); i++){
		DSERIAL.write(*ptr);
		ptr++;
	}
	int count;
	// read and write the bulk
	int k = 49;

	for(int j = 0; j < numFrames; j++){

		while(1) {
			if (DSERIAL.available() > 0){
				if (DSERIAL.read() == EOT)
					break;
			}
		}

		for(int i = 0; i < bufSize - crcSize; i++){
			oneKbuf[i] = (unsigned char)fin.read();
		}

		// add in the crc
		crcClcData.crcInt = crcbitbybitfast(oneKbuf, bufSize - crcSize);

		for(int i = 0; i < crcSize; i++){
			oneKbuf[bufSize - crcSize + i] = crcClcData.crcArray[i];
		}

		count = 0;
		while (1) {
			for(int i = 0; i < bufSize; i++){
				DSERIAL.write(oneKbuf[i]);
			}

			//do the sync stuff
			while(1) {
				if (DSERIAL.available() > 0){
					if (DSERIAL.read() == SYNC)
						break;
				}
			}
			DSERIAL.write(SYNC);

			while(1) {
				if (DSERIAL.available() > 0){
					tmpCrc = DSERIAL.read();
					break;
				}
			}

			if (tmpCrc == SOK){
				break;
			} else if (tmpCrc == NOK){
				count++;
				if (count == RETRYCOUNT) break;
			}
		}  // infinite loop
	}


	// remainder
	while(1) {
		if (DSERIAL.available() > 0){
			if (DSERIAL.read() == EOT)
				break;
		}
	}

	for(int i = 0; i < remainder; i++){
		oneKbuf[i] = (unsigned char)fin.read();
	}

	// add in the crc
	crcClcData.crcInt = crcbitbybitfast(oneKbuf, remainder);

	for(int i = 0; i < crcSize; i++){
		oneKbuf[remainder + i] = crcClcData.crcArray[i];
	}

	count = 0;
	while (1) {
		for(int i = 0; i < remainder + crcSize; i++){
			DSERIAL.write(oneKbuf[i]);
		}

		//do the sync stuff
		while(1) {
			if (DSERIAL.available() > 0){
				if (DSERIAL.read() == SYNC)
					break;
			}
		}
		DSERIAL.write(SYNC);

		while(1) {
			if (DSERIAL.available() > 0){
				tmpCrc = DSERIAL.read();
				break;
			}
		}

		if (tmpCrc == SOK){
			break;
		} else if (tmpCrc == NOK){
			count++;
			if (count == RETRYCOUNT) break;
		}
	}  // infinite loop
	fin.close();
}


