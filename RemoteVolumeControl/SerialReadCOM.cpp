#include "SerialReadCOM.h"



SerialReadCOM::SerialReadCOM(char * portName)
{
	this->connected = false;

	this->hSerial = CreateFile(portName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (this->hSerial == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			printf("ERROR: Handle was not attached. Reason: %s not available. \n", portName);
		else
			printf("ERROR!!!");
	}
	else {
		DCB dcbSerialParams = { 0 };

		if (!GetCommState(this->hSerial, &dcbSerialParams)) {
			printf("failed to get current serial parameters");
		}
		else {
			dcbSerialParams.BaudRate = CBR_9600;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;
			dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(hSerial, &dcbSerialParams)) {
				printf("ALERT: Could not set Serial Port parameters");
			}
			else
			{
				this->connected = true;
				PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}
}

SerialReadCOM::~SerialReadCOM()
{
	if (this->connected)
	{
		this->connected = false;
		CloseHandle(this->hSerial);
	}
}

int SerialReadCOM::ReadData(char * buff, unsigned int nbChar)
{
	DWORD bytesRead;
	unsigned int toRead;

	ClearCommError(this->hSerial, &this->errors, &this->status);

	if (this->status.cbInQue > 0) {

		if (this->status.cbInQue > nbChar)
			toRead = nbChar;
		else
			toRead = this->status.cbInQue;

		if (ReadFile(this->hSerial, buff, toRead, &bytesRead, NULL))
		{
			return bytesRead;
		}
	}

	return 0;
}

bool SerialReadCOM::WriteData(char * buffer, unsigned int nbChar)
{
	DWORD bytesSend;

	if (!WriteFile(this->hSerial, (void *)buffer, nbChar, &bytesSend, 0))
	{
		ClearCommError(this->hSerial, &this->errors, &this->status);
		return false;
	}
	else
		return true;
}

void SerialReadCOM::closeConnection()
{
	if (this->connected) {
		this->connected = false;
		CloseHandle(this->hSerial);
	}

}

bool SerialReadCOM::isConnected()
{
	return this->connected;
}
