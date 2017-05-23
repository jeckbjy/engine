#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

enum AudioSourceState
{
	AUDIO_STATE_PLAYING,
	AUDIO_STATE_PAUSED,
	AUDIO_STATE_STOPPED
};

class AudioClip;
class CUTE_CORE_API AudioSource : public Object
{
	DECLARE_RTTI(AudioSource, Object, OID_ANY)
public:
	AudioSource();
	~AudioSource();

	virtual void play() = 0;
	virtual void stop() = 0;
	virtual void pause() = 0;

	virtual void setClip(AudioClip* clip);
	virtual void setPosition(const Vector3& position);
	virtual void setVelocity(const Vector3& velocity);
	virtual void setVolume(float volume);
	virtual void setPitch(float pitch);
	virtual void setLooping(bool loop);
	virtual void setPriority(int priority);
	virtual void setMinDistance(float distance);
	virtual void setAttenuation(float attenuation);
	virtual void setTime(float time) = 0;

	virtual float	getTime() const = 0;
	virtual AudioSourceState getState() const = 0;

	const Vector3&	getPosition() const;
	const Vector3&	getVelocity() const;
	float			getVolume() const;
	float			getPitch() const;
	float			getMinDistance() const;
	float			getAttenuation() const;
	bool			isLooping() const;

protected:
	AudioClip*	m_clip;
	Vector3		m_pos;
	Vector3		m_velocity;
	float		m_volume;
	float		m_pitch;
	bool		m_loop;
	int			m_priority;
	float		m_minDistance;
	float		m_attenuation;	// Ï¡±¡
};

//////////////////////////////////////////////////////////////////////////
// inline
//////////////////////////////////////////////////////////////////////////
inline const Vector3& AudioSource::getPosition() const
{
	return m_pos;
}

inline const Vector3& AudioSource::getVelocity() const
{
	return m_velocity;
}

inline float AudioSource::getVolume() const
{
	return m_volume;
}

inline float AudioSource::getPitch() const
{
	return m_pitch;
}

inline float AudioSource::getMinDistance() const
{
	return m_minDistance;
}

inline float AudioSource::getAttenuation() const
{
	return m_attenuation;
}

inline bool AudioSource::isLooping() const
{
	return m_loop;
}

CUTE_NS_END
