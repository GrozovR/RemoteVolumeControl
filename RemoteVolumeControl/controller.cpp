#include "SerialReadCOM.h"
#include <cstring>


void main() {
	printf("Welcome to the serial test app!\n\n");

	SerialReadCOM* SP = new SerialReadCOM("\\\\.\\COM4");

	if (SP->isConnected())
		printf("Arduino Connected!");

	char incomingData[256] = "";

	char buttonPlus[11] = "FFA857";
	char buttonMinus[11] = "FFE01F";
	char delims[] = "\t\n";
	char *context = nullptr;

	int dataLenght = 255;
	int readResult = 0;

	while (SP->isConnected())
	{
		readResult = SP->ReadData(incomingData, dataLenght);
		incomingData[readResult] = 0;

		//if(strcmp(incomingData, buttonPlus) == 0)
		//	printf("+");
		//else if(strcmp(incomingData, buttonMinus) == 0)
		//	printf("-");
		//else
		printf("%s", incomingData);


		char *tok;

		tok = strtok_s(incomingData, delims, &context);

		while (tok){
			printf("%s \n", tok);



			if (strcmp(tok, buttonPlus) == 0)
				printf("+\n\n");
			else if (strcmp(tok, buttonMinus) == 0)
				printf("-\n\n");

			tok = strtok_s(nullptr, delims, &context);
		}



		Sleep(500);
	}



	system("pause");
}