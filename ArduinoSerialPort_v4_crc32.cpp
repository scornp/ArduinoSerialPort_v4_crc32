// Do not remove the include below
#include "ArduinoSerialPort_v4_crc32.h"

#include "common.h"


//#include <SdFatUtil.h>

const int bufs = 10;
unsigned char myArray[bufs];



SdFat sd;
//SdFile fout;

const int bufSize = 64;
unsigned char oneKbuf[bufSize];

int32_t fileSize = 0x0000ffff;
int32_t crcX = 0xffffffff;
int32_t poly = 0x11223344;
int32_t initX = 0x55667788;

void help(void);


uint32_t pos, pos_old;

//The setup function is called once at startup of the sketch
void setup()
{
	int chipSelect = 53;
	//	  if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt(&DSERIAL);
	//	  if (!sd.init(SPI_HALF_SPEED, chipSelect)) sd.initErrorHalt(&DSERIAL);
	if(!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt(&DSERIAL);

	int32_t error = checkMaskEtc();

	Serial.begin(115200);
//	Serial.begin(57600);
//	Serial.begin(9600);

//	delay(200);
//	Serial.print("<Arduino> : here \n");
//	Serial.write(EOT);
//	Serial.flush();
	sd.vwd()->rewind();
	pos = sd.vwd()->curPosition();
	pos_old = sd.vwd()->curPosition();

}

void help(void)
{
	DSERIAL.println("Available Commands:");
	DSERIAL.println("HELP     - Print this list of commands");
	DSERIAL.println("DIR      - List files - alternate LS");
	DSERIAL.println("PWD      - Print current working directory");
	DSERIAL.println("CD       - Change current working directory");
	DSERIAL.println("DEL file - Delete file - alternate RM");
	DSERIAL.println("MD  dir  - Create dir - alternate MKDIR");
	DSERIAL.println("RD  dir  - Delete dir - alternate RMDIR");
	DSERIAL.println("ATOH f1 f2 - Send file from Arduino to host");
	DSERIAL.println("HTOA f1 f2 - Send file from Host to Arduino");
}



dir_t dir;
char name[13];


void loop()
{
	unsigned char *cmd;
	unsigned char *param;
	unsigned char *cmdStr;

	cmd = 0;
	param = 0;
	cmdStr = 0;

	int nargs = 0;
	unsigned char *argv[10];
	int count = 0;

/*
 			DSERIAL.println("<Arduino> : here ");
		//	DSERIAL.write(EOT);
			delay(100);
			return;
*/

	if (DSERIAL.available() > 0) {

		// reset buffer
		for(unsigned int i = 0; i < sizeof(oneKbuf); i++) oneKbuf[i] = '\0';

		cmdStr = oneKbuf;

		count = getInput(cmdStr);
		//	copy full buffer

		if (count > 0){
			cmdStr = oneKbuf;

			getArguments(cmdStr, strlen((char *)cmdStr), &nargs, argv);

			// agree tokenizer
			if (!strcmp((char *)argv[0], "HELP")) {
				help();
				DSERIAL.write(EOT);
			} else if (!strcmp((char *)argv[0], "ODD")) {
				DSERIAL.print("<Arduino> Command unknown ... ");
				DSERIAL.print((char *)cmd);
				DSERIAL.print(" with parameter ");
				DSERIAL.println((char *)param);
				DSERIAL.write(EOT);
			} else if (!strcmp((char *)argv[0], "DIR") || !strcmp((char *)argv[0], "LS")) {
				DSERIAL.print("<Arduino> DIR listing ...  ");
				DSERIAL.println((char *)argv[0]);

				// format file name
				SdFile::dirName(&dir, name);

				// remember position in directory
				pos = sd.vwd()->curPosition();
				sd.vwd()->seekSet(pos);
				sd.vwd()->rewind();
				while (sd.vwd()->readDir(&dir) == sizeof(dir)) {
					// read next directory entry in current working directory
					SdFile::dirName(&dir, name);
					DSERIAL.print(name);
					if(strlen(name) < 8) DSERIAL.print('\t');
					DSERIAL.print('\t');
					if (!(dir.attributes & DIR_ATT_DIRECTORY)) {
						ultoa(dir.fileSize, name, DEC);
						DSERIAL.println(name);
					} else {
						DSERIAL.println("<DIR>");
					}
				}
				DSERIAL.print(" with parameter ");
				DSERIAL.println((char *)argv[1]);
				DSERIAL.write(EOT);
			} else if (!strcmp((char *)argv[0], "CD")) {
				pos_old = sd.vwd()->curPosition();
				if(!sd.chdir((char *)argv[1], true)) {
					DSERIAL.print("changed directory to ");
				} else {
					DSERIAL.print("Not changed directory changed to ");
				}
				DSERIAL.println((char *)argv[1]);
				DSERIAL.write(EOT);
			} else if (!strcmp((char *)argv[0], "PWD")) {
				sd.vwd()->getName(name, 13);
				DSERIAL.print("Current working directory is ");
				DSERIAL.println((char *)name);
				DSERIAL.write(EOT);
			} else if (!strcmp((char *)argv[0], "DEL") || !strcmp((char *)argv[0], "RM")) {
				DSERIAL.println("<Arduino> : deleting file from host ...");
				DSERIAL.println((char *)argv[1]);
				DSERIAL.println("<Arduino> : entering binary send mode");
				if (!sd.remove((char *)argv[1])) {
					DSERIAL.print("Failed to delete file ");
					DSERIAL.println((char *)argv[1]);
				} else {
					DSERIAL.print("File ");
					DSERIAL.print((char *)argv[1]);
					DSERIAL.println(" deleted");
				}

				DSERIAL.write(EOT);

			} else if (!strcmp((char *)argv[0], "RD")) {
				DSERIAL.println("<Arduino> : deleting dir ...");
				DSERIAL.print((char *)argv[1]);
				DSERIAL.println("<Arduino> : entering binary send mode");
				if (!sd.rmdir((char *)argv[1])) {
					DSERIAL.print("Failed to delete dir ");
					DSERIAL.println((char *)argv[1]);
				} else {
					DSERIAL.print("Dir ");
					DSERIAL.print((char *)argv[1]);
					DSERIAL.println(" deleted");
				}

				DSERIAL.write(EOT);

			} else if (!strcmp((char *)argv[0], "MD")) {
				DSERIAL.print("<Arduino> : creating dir ...");
				DSERIAL.println((char *)argv[1]);
				DSERIAL.println("<Arduino> : entering binary send mode");
				if (!sd.mkdir((char *)argv[1])) {
					DSERIAL.print("Failed to create dir ");
					DSERIAL.println((char *)argv[1]);
				} else {
					DSERIAL.print("Dir ");
					DSERIAL.print((char *)argv[1]);
					DSERIAL.println(" create");
				}

				DSERIAL.write(EOT);

			} else if (!strcmp((char *)argv[0], "ATOH")) {
				sendFile(&sd, &nargs, argv, oneKbuf, bufSize);
				DSERIAL.write(EOT);
			} else if (!strcmp((char *)argv[0], "HTOA")) {
				recvFile(oneKbuf, bufSize);
				DSERIAL.write(EOT);
			} else {
				DSERIAL.print("<Arduino> Command unknown ... ");
				DSERIAL.println((char *)argv[0]);
				DSERIAL.write(EOT);
			}
		}
		delay(100);
	}
}


