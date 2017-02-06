#include "SerialReadCOM.h"

void main() {
	printf("Welcome to the serial test app!\n\n");

	SerialReadCOM* SP = new SerialReadCOM("\\\\.\\COM4");

	if (SP->isConnected())
		printf("Arduino Connected!");
	
	char incomingData[256] = "";
	//char buttonPlus[11] = "FFA857\t\n";
	//char buttonMinus[11] = "FFE01F\t\n";

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

		
		Sleep(500);
	}
	
	system("pause");
}