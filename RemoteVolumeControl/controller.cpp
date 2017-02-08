#include "SerialReadCOM.h"
#include <cstring>
#pragma comment (lib, "winmm")

const char buttonPlus[] = "FFA857";
const char buttonMinus[] = "FFE01F";
const char delims[] = "\r\n";
const int dataLenght{ 255 };
char incomingData[256] = "";
char *context = nullptr;
short volumeLevel{ 60 };


struct mode 
{
	enum modes
	{
		VOLUMEUP_MODE,
		VOLUMEDOWN_MODE,
		IGNORE_MODE
	} currentMode;
} modeApp;


void volumeUp()
{
	if (volumeLevel < 255)
	{
		volumeLevel++;
		MMRESULT res = waveOutSetVolume(0, (DWORD)volumeLevel);
	}
}

void volumeDown()
{
	if (volumeLevel >0){
		volumeLevel--;
		waveOutSetVolume(0, (DWORD)volumeLevel);
	}	
}


void main() {
	printf("Welcome to Remote Volume Control app!\n\n");
	
	//TODO: input port number

	SerialReadCOM* SP = new SerialReadCOM("\\\\.\\COM3");

	if (SP->isConnected())
		printf("Arduino Connected!");

	int readResult{ 0 };
	modeApp.currentMode = mode::IGNORE_MODE;

	while (SP->isConnected())
	{
		readResult = SP->ReadData(incomingData, dataLenght);
		incomingData[readResult] = 0;

		//printf("%s", incomingData);

		char *tok;
		tok = strtok_s(incomingData, delims, &context);

		while (tok){

			if (strcmp(tok, buttonPlus) == 0) {
				//printf("+\n\n");
				modeApp.currentMode = mode::VOLUMEUP_MODE;
				volumeUp();
			}				
			else if (strcmp(tok, buttonMinus) == 0) {
				//printf("-\n\n");
				modeApp.currentMode = mode::VOLUMEDOWN_MODE;
				volumeDown();
			}
			else if(strcmp(tok,"FFFFFFFF") == 0){
				//printf("%s\n", tok);
				if (modeApp.currentMode == mode::IGNORE_MODE)
					break;
				else if (modeApp.currentMode == mode::VOLUMEUP_MODE)
					volumeUp();
				else if (modeApp.currentMode == mode::VOLUMEDOWN_MODE)
					volumeDown();
			}
			else
				modeApp.currentMode = mode::IGNORE_MODE;

			tok = strtok_s(nullptr, delims, &context);
		}
		
		Sleep(500);
	}

	
	system("pause");
}