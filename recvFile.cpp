/*
 * recvFile.cpp
 *
 *  Created on: 17 Mar 2019
 *      Author: Roger
 */


#include "common.h"

#define DSERIAL Serial


void recvFile(unsigned char *oneKbuf, int bufSize){
	header recv;
	union crcOverlap crcClcData;
	union crcOverlap crcRcvData;
	SdFile fout;

	for(int j = 0; j < bufs; j++) myArray[j] = 'a';

	int wlen;
	unsigned char ch;

	while(1) {
		ch = DSERIAL.read();
		if (ch == BOT) break;
	}
	DSERIAL.write(BOT);



	readStream((unsigned char* )&recv, sizeof(recv));


	unsigned char ArduinoSaveAs[20];

	strcpy(ArduinoSaveAs, recv.fileName);

	int32_t numFrames;
	int remainder;
	int crcSize = 4;
	int32_t fileSize = recv.fileSize;
	numFrames = fileSize/(bufSize - crcSize);
	remainder = fileSize % (bufSize - crcSize);

	fout.open(ArduinoSaveAs, O_RDWR | O_CREAT );
	uint32_t tmpcrc[10];

	unsigned char *ptr;

	// read and write the bulk frames
	for(int32_t j = 0; j < numFrames; j++){

		//-rnp get serial data loop until succesful send
		int count = 0;
		while(1) {
			//DSERIAL.println("<Arduino><recvFile><01> : recvFile");
			DSERIAL.write(EOT);

			int rcv = readStream1(oneKbuf, bufSize);

			crcClcData.crcInt = crcbitbybitfast(oneKbuf, bufSize - 4);
			for(int k = 0; k < 4; k++) crcRcvData.crcArray[k] = oneKbuf[bufSize - 4 + k];

			//-rnp added syncing for crc check
			//-rnp do crc check
		//	DSERIAL.println("<Arduino><recvFile><02> : recvFile");
		//	DSERIAL.write(EOT);

			DSERIAL.write(SYNC);
			if (crcClcData.crcInt == crcRcvData.crcInt) {
				DSERIAL.write(SOK);
				break;
			} else {
				DSERIAL.write(NOK);
				count++;
				if (count == 10) break;
			}
		}

		// write data to file
		ptr = oneKbuf;
		for(int32_t i = 0; i < bufSize  - crcSize; i++){
			fout.write(*ptr);
			ptr++;
		}
	}


//	DSERIAL.print("\n<Arduino><recvFile><01> : Remainder ");

	// now do the remainder
	int count = 0;
	while(1) {


		readStream1(oneKbuf, remainder + crcSize);

		crcClcData.crcInt = crcbitbybitfast(oneKbuf, remainder);
		for(int k = 0; k < crcSize; k++) crcRcvData.crcArray[k] = oneKbuf[remainder + k];

		//-rnp added syncing for crc check
		//-rnp do crc check

		DSERIAL.write(SYNC);
		if (crcClcData.crcInt == crcRcvData.crcInt) {
			DSERIAL.write(SOK);
			break;
		} else {
			DSERIAL.write(NOK);
			count++;
			if (count == 10) break;
		}
	}

	// write remainder to file
	ptr = oneKbuf;
	for(int32_t i = 0; i < remainder; i++){
		fout.write(*ptr);
		ptr++;
	}


	fout.close();
	DSERIAL.print("\n<Arduino><recvFile><02> : Remainder ");

}

