#include "OAL_AudioListener.h"
#include "OAL_Audio.h"

CUTE_NS_BEGIN

OALAudioListener::OALAudioListener(ALCdevice* device)
	: m_context(NULL)
{
	m_context = alcCreateContext(device, NULL);
	rebuild();
}

OALAudioListener::~OALAudioListener()
{
	if (m_context != NULL)
	{
		alcDestroyContext(m_context);
		m_context = NULL;
	}
}

void OALAudioListener::setPosition(const Vector3& pos)
{
	AudioListener::setPosition(pos);
	
	setCurrentContext();
	updatePosition();
}

void OALAudioListener::setDirection(const Vector3& dir)
{
	AudioListener::setDirection(dir);

	setCurrentContext();
	updateOrientation();
}

void OALAudioListener::setUp(const Vector3& up)
{
	AudioListener::setUp(up);

	setCurrentContext();
	updateOrientation();
}

void OALAudioListener::setVelocity(const Vector3& velocity)
{
	AudioListener::setVelocity(velocity);

	setCurrentContext();
	updateVelocity();
}

void OALAudioListener::setCurrentContext()
{
	alcMakeContextCurrent(m_context);
}

void OALAudioListener::rebuild()
{
	float volume = gOALAudio().getVolume();

	setCurrentContext();
	updatePosition();
	updateOrientation();
	updateVelocity();
	updateVolume(volume);
}

void OALAudioListener::updatePosition()
{
	alListener3f(AL_POSITION, m_position.x, m_position.y, m_position.z);
}

void OALAudioListener::updateOrientation()
{
	float orientation[6] =
	{
		m_direction.x, m_direction.y, m_direction.z,
		m_up.x, m_up.y, m_up.z
	};

	alListenerfv(AL_ORIENTATION, orientation);
}

void OALAudioListener::updateVelocity()
{
	alListener3f(AL_VELOCITY, m_velocity.x, m_velocity.y, m_velocity.z);
}

void OALAudioListener::updateVolume(float volume)
{
	alListenerf(AL_GAIN, volume);
}

CUTE_NS_END
