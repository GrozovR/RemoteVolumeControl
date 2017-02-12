#include <cstring>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "VolumeController.h"
#include "SerialReadCOM.h"

//TODO: 1 input port number
// 2 maybe problem, need to check exit without couninitialize
// 3 string 40 in this file
// 4 fast volume changing

const char buttonPlus[] = "FFA857"; //button +
const char buttonMinus[] = "FFE01F"; //button -
const char buttonExit[] = "FF5AA5"; //button 6

const char delims[] = "\r\n";
const int dataLenght{ 255 };
char incomingData[256] = "";
char *context = nullptr;


void main() {
	printf("Welcome to Remote Volume Control app!\n\n");
	
	SerialReadCOM* SP = new SerialReadCOM("\\\\.\\COM3");

	if (SP->isConnected())
		printf("Arduino Connected!\n");

	int readResult{ 0 };


	CoInitialize(NULL);

	VolumeController volumeControl;

	while (SP->isConnected())
	{
		readResult = SP->ReadData(incomingData, dataLenght);
		incomingData[readResult] = 0; //what is it ??

		char *tok;
		tok = strtok_s(incomingData, delims, &context);

		while (tok) {

			if (strcmp(tok, buttonPlus) == 0) {
				//printf("+\n\n");
				volumeControl.setMode(volumeModes::VOLUMEUP_MODE);
				volumeControl.volumeUp();
			}
			else if (strcmp(tok, buttonMinus) == 0) {
				//printf("-\n\n");
				volumeControl.setMode(volumeModes::VOLUMEDOWN_MODE);
				volumeControl.volumeDown();
			}
			else if (strcmp(tok, buttonExit) == 0) {
				volumeControl.Release();
				SP->closeConnection();
				break;
			}
			else if (strcmp(tok, "FFFFFFFF") == 0) {
				//printf("%s\n", tok);
				volumeControl.continueChangeVolume();
			}
			else {
				volumeControl.setMode(volumeModes::IGNORE_MODE);
			}

			tok = strtok_s(nullptr, delims, &context);
		}

		Sleep(200);
	}

	CoUninitialize();
}