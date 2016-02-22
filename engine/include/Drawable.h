#pragma once
#include "Component.h"
#include "AABox.h"
#include "Bits.h"

CU_NS_BEGIN

class Octant;
class CU_API Drawable : public Component, public ListNode<Drawable, 2>
{
	DECLARE_RTTI(Drawable, Component, "DRAW")

	enum
	{
		FLAG_OCCLUDER = 0x01,
		FLAG_OCCLUDEE = 0x02,
	};
public:
	Drawable();
	~Drawable();

	const AABox& getWorldBox() const;

	bool isOccluder() const { return m_flags[FLAG_OCCLUDEE]; }
	bool isOccludee() const { return m_flags[FLAG_OCCLUDEE]; }

protected:
	friend class Octant;
	AABox	m_localBox;
	AABox	m_worldBox;
	Mask8	m_flags;
	Octant*	m_octant;
};

CU_NS_END