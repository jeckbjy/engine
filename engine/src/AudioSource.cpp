//! Audio
#include "Cute/AudioSource.h"
#include "Cute/Math.h"

CUTE_NS_BEGIN

AudioSource::AudioSource()
	: m_clip(NULL)
	, m_pos(Vector3::ZERO)
	, m_velocity(Vector3::ZERO)
	, m_volume(1.0f)
	, m_pitch(1.0f)
	, m_loop(false)
	, m_priority(0)
	, m_minDistance(1.0f)
	, m_attenuation(1.0f)
{
}

AudioSource::~AudioSource()
{
}

void AudioSource::setClip(AudioClip* clip)
{
	m_clip = clip;
}

void AudioSource::setPosition(const Vector3& position)
{
	m_pos = position;
}

void AudioSource::setVelocity(const Vector3& velocity)
{
	m_velocity = velocity;
}

void AudioSource::setVolume(float volume)
{
	m_volume = Math::clamp01(volume);
}

void AudioSource::setPitch(float pitch)
{
	m_pitch = pitch;
}

void AudioSource::setLooping(bool loop)
{
	m_loop = loop;
}

void AudioSource::setPriority(int priority)
{
	m_priority = priority;
}

void AudioSource::setMinDistance(float distance)
{
	m_minDistance = distance;
}

void AudioSource::setAttenuation(float attenuation)
{
	m_attenuation = attenuation;
}

CUTE_NS_END
