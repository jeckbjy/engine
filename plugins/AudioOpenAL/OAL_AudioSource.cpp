#include "OAL_AudioSource.h"

CUTE_NS_BEGIN

void OALAudioSource::play()
{
	m_state = AUDIO_STATE_PLAYING;

}

void OALAudioSource::stop()
{
}

void OALAudioSource::pause()
{
}

void OALAudioSource::setClip(AudioClip* clip)
{

}

void OALAudioSource::setPosition(const Vector3& position)
{
	AudioSource::setPosition(position);

	//
}

void OALAudioSource::setVelocity(const Vector3& velocity)
{
	AudioSource::setVelocity(velocity);

}

void OALAudioSource::setVolume(float volume)
{
	AudioSource::setVolume(volume);

}

void OALAudioSource::setPitch(float pitch)
{
	AudioSource::setPitch(pitch);

}

void OALAudioSource::setLooping(bool loop)
{
	AudioSource::setLooping(loop);
}

void OALAudioSource::setPriority(int priority)
{
	AudioSource::setPriority(priority);

	// Do nothing, OpenAL doesn't support priorities (perhaps emulate the behaviour by manually disabling sources?)
}

void OALAudioSource::setMinDistance(float distance)
{
	AudioSource::setMinDistance(distance);

}

void OALAudioSource::setAttenuation(float attenuation)
{
	AudioSource::setAttenuation(attenuation);

}

CUTE_NS_END
