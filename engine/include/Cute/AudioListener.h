#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

class CUTE_CORE_API AudioListener : public Object
{
	DECLARE_RTTI(AudioListener, Object, OID_ANY)
public:
	virtual ~AudioListener(){}

	virtual void setPosition(const Vector3& pos);
	virtual void setDirection(const Vector3& dir);
	virtual void setUp(const Vector3& up);
	virtual void setVelocity(const Vector3& velocity);

	const Vector3& getPosition() const { return m_position; }
	const Vector3& getDirection() const { return m_direction; }
	const Vector3& getUp() const { return m_up; }
	const Vector3& getVelocity() const { return m_velocity; }

protected:
	AudioListener();

	Vector3 m_position;
	Vector3 m_direction;
	Vector3	m_up;
	Vector3 m_velocity;
};

CUTE_NS_END
