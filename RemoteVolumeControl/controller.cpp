#include "SerialReadCOM.h"
#include <cstring>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

const char buttonPlus[] = "FFA857";
const char buttonMinus[] = "FFE01F";
const char delims[] = "\r\n";
const int dataLenght{ 255 };
char incomingData[256] = "";
char *context = nullptr;
double volumeLevel{ 0.5 };



//enter value from 0.0 to 1.0
bool changeVolume(double nVolume, bool bScalar)
{
	HRESULT hr = NULL;
	bool decibels = false;
	bool scalar = false;
	double newVolume = nVolume;

	CoInitialize(NULL);

	IMMDeviceEnumerator* deviceEnumerator = NULL;

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(LPVOID *)&deviceEnumerator);

	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	// -------------------------
	float currentVolume = 0;
	endpointVolume->GetMasterVolumeLevel(&currentVolume);

	hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);


	if (bScalar == false)
	{
		hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
	}
	else if (bScalar == true)
	{
		hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
	}
	endpointVolume->Release();



	CoUninitialize();
	return false;
}



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
	if (volumeLevel < 1.0)
	{
		volumeLevel += 0.01;
		changeVolume(volumeLevel,true);
	}
}

void volumeDown()
{
	if (volumeLevel > 0.0){
		volumeLevel -= 0.01;
		changeVolume(volumeLevel,true);
	}	
}


void main() {
	printf("Welcome to Remote Volume Control app!\n\n");
	
	//TODO: input port number, get initial volume level

	SerialReadCOM* SP = new SerialReadCOM("\\\\.\\COM4");

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
		
		Sleep(100);
	}

	
	system("pause");
}