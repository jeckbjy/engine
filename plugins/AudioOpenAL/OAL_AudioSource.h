#pragma once
#include "OAL_API.h"
#include "Cute/AudioSource.h"

CUTE_NS_BEGIN

class CUTE_OAL_API OALAudioSource : public AudioSource
{
public:
	OALAudioSource();
	~OALAudioSource();

	void play() OVERRIDE;
	void stop() OVERRIDE;
	void pause() OVERRIDE;

	void setClip(AudioClip* clip) OVERRIDE;
	void setPosition(const Vector3& position) OVERRIDE;
	void setVelocity(const Vector3& velocity) OVERRIDE;
	void setVolume(float volume) OVERRIDE;
	void setPitch(float pitch) OVERRIDE;
	void setLooping(bool loop) OVERRIDE;
	void setPriority(int priority) OVERRIDE;
	void setMinDistance(float distance) OVERRIDE;
	void setAttenuation(float attenuation) OVERRIDE;
	void setTime(float time) OVERRIDE;
	float getTime() const OVERRIDE;
	AudioSourceState getState() const OVERRIDE;

private:
	float				m_savedTime;
	AudioSourceState	m_savedState;
	AudioSourceState	m_state;
};

CUTE_NS_END
