#include "VolumeController.h"


VolumeController::VolumeController()
{
	HRESULT hr = NULL;
	bool decibels = false;
	bool scalar = false;
	double newVolume;


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

	
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&m_endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	hr = m_endpointVolume->GetMasterVolumeLevelScalar(&m_volumeLevel);
}


VolumeController::~VolumeController()
{

}

void VolumeController::volumeUp()
{
	if (m_volumeLevel < 1.0)
	{
		m_volumeLevel += 0.01;
		changeVolume(m_volumeLevel);
	}
}

void VolumeController::volumeDown()
{
	if (m_volumeLevel > 0.0) {
		m_volumeLevel -= 0.01;
		changeVolume(m_volumeLevel);
	}
}

void VolumeController::continueChangeVolume()
{
	if (m_mode == volumeModes::VOLUMEUP_MODE)
		volumeUp();
	else if (m_mode == volumeModes::VOLUMEDOWN_MODE)
		volumeDown();
}

void VolumeController::changeVolume(float nVolume)
{
	HRESULT hr = NULL;
	float newVolume = nVolume;

	hr = m_endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
}

void VolumeController::setMode(volumeModes mod)
{
	m_mode = mod;
}

void VolumeController::Release()
{
	if (m_endpointVolume) {
		m_endpointVolume->Release();
		m_endpointVolume = nullptr;
	}

}
