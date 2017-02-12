#ifndef VOLUMECONTROLLER_H
#define VOLUMECONTROLLER_H

#include <mmdeviceapi.h>
#include <endpointvolume.h>


enum class volumeModes
{
	VOLUMEUP_MODE,
	VOLUMEDOWN_MODE,
	IGNORE_MODE
};

class VolumeController
{
public:
	VolumeController();
	~VolumeController();

	void volumeUp();
	void volumeDown();
	void continueChangeVolume();
	void setMode(volumeModes mod);

	void Release();

private:
	void changeVolume(float nVolume);
	
	float m_volumeLevel{ 0.5f };

	IAudioEndpointVolume* m_endpointVolume{ nullptr };
	
	volumeModes m_mode = volumeModes::IGNORE_MODE;
};

#endif // !VOLUMECONTROLLER_H


