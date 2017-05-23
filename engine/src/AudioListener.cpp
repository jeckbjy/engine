//! Audio
#include "Cute/AudioListener.h"

CUTE_NS_BEGIN

AudioListener::AudioListener()
	: m_position(Vector3::ZERO)
	, m_direction(-Vector3::UNIT_Z)
	, m_up(Vector3::UNIT_Y)
	, m_velocity(Vector3::ZERO)
{
}

void AudioListener::setPosition(const Vector3& pos)
{
	m_position = pos;
}

void AudioListener::setDirection(const Vector3& dir)
{
	m_direction = dir;
}

void AudioListener::setUp(const Vector3& up)
{
	m_up = up;
}

void AudioListener::setVelocity(const Vector3& velocity)
{
	m_velocity = velocity;
}

CUTE_NS_END