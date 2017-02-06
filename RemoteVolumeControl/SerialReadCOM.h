#ifndef SERIALREADCOM_H_2017
#define SERIALREADCOM_H_2017

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ARDUINO_WAIT_TIME 2000

class SerialReadCOM
{
private:
	HANDLE hSerial;
	bool connected;
	COMSTAT status;
	DWORD errors;

public:
	SerialReadCOM(char* portName);
	~SerialReadCOM();

	int ReadData(char *buff, unsigned int nbChar);

	bool WriteData(char *buffer, unsigned int nbChar);

	bool isConnected();
};

#endif // !SERIALREADCOM_H_2017

