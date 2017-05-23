#pragma once
#include "OAL_API.h"
#include "Cute/Audio.h"

CUTE_NS_BEGIN

class CUTE_OAL_API OALAudio : public Audio
{
public:
	OALAudio();
	~OALAudio();

	void setVolume(float volume) OVERRIDE;
	void setPaused(bool paused) OVERRIDE;
	void setActiveDevice(const String& name) OVERRIDE;

	float getVolume() const OVERRIDE;
	bool  isPaused() const OVERRIDE;

	String getActiveDevice() const OVERRIDE;
	String getDefaultDevice() const OVERRIDE;
	const DeviceList& getAllDevices() const OVERRIDE;

	ALCdevice* getALCDevice() const;

private:
	bool isExtensionSupported(const String& ext) const;

private:
	ALCdevice*	m_device;
	DeviceList	m_allDevices;
	String		m_defaultDevice;
	String		m_activeDevice;

	float		m_volume;
	bool		m_paused;
};

OALAudio& gOALAudio();

//////////////////////////////////////////////////////////////////////////
// inline
//////////////////////////////////////////////////////////////////////////
inline ALCdevice* OALAudio::getALCDevice() const
{
	return m_device;
}

CUTE_NS_END
