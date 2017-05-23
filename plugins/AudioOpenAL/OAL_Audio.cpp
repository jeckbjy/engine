#include "OAL_Audio.h"
#include "Cute/Math.h"

CUTE_NS_BEGIN

OALAudio& gOALAudio()
{
	return static_cast<OALAudio&>(OALAudio::instance());
}

OALAudio::OALAudio()
	: m_device(NULL)
	, m_volume(1.0f)
	, m_paused(false)
{
	// 枚举所有设备
	bool enumeratedDevices;
	if (isExtensionSupported("ALC_ENUMERATE_ALL_EXT"))
	{
		const ALCchar* defaultDevice = alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
		const ALCchar* devices		 = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
		m_defaultDevice = defaultDevice;

		while (true)
		{
			if (*devices == 0)
			{
			}
		}

		enumeratedDevices = true;
	}
	else
	{
		enumeratedDevices = false;
	}

	m_activeDevice = m_defaultDevice;
	m_device = alcOpenDevice(enumeratedDevices ? m_defaultDevice.c_str() : NULL);
}

OALAudio::~OALAudio()
{
	if (m_device != NULL)
	{
		alcCloseDevice(m_device);
		m_device = NULL;
	}
}

void OALAudio::setVolume(float volume)
{
	m_volume = Math::clamp01(volume);
}

void OALAudio::setPaused(bool paused)
{
	if (m_paused == paused)
		return;

	m_paused = paused;
	// 设置
}

void OALAudio::setActiveDevice(const String& name)
{
	if (m_device != NULL)
	{
		alcCloseDevice(m_device);
		m_device = NULL;
	}

}

float OALAudio::getVolume() const
{
	return m_volume;
}

bool OALAudio::isPaused() const
{
	return m_paused;
}

String OALAudio::getActiveDevice() const
{
	return m_activeDevice;
}

String OALAudio::getDefaultDevice() const
{
	return m_defaultDevice;
}

bool OALAudio::isExtensionSupported(const String& ext) const
{
	bool isALC = ext.length() > 2 && ext.substr(0, 3) == "ALC";
	if (isALC)
	{
		if (m_device != NULL)
			return alcIsExtensionPresent(m_device, ext.c_str()) != AL_FALSE;
		else
			return false;
	}
	else
	{
		return alIsExtensionPresent(ext.c_str()) != AL_FALSE;
	}
}

CUTE_NS_END
