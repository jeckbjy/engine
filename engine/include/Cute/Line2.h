#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector2.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Line2
{
public:
	Line2();
	~Line2();

	const Vector2& getOrigin(void) const { return m_origin; }
	const Vector2& getDirection(void) const { return m_direction; }

	void setOrigin(const Vector2& value) { m_origin = value; }
	void setDirection(const Vector2& value) { m_direction = value; }

private:
	Vector2 m_origin;
	Vector2 m_direction;
};

CUTE_NS_END
