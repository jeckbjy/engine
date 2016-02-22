#include "Drawable.h"

CU_NS_BEGIN

Drawable::Drawable()
:m_octant(NULL)
{

}

Drawable::~Drawable()
{

}

const AABox& Drawable::getWorldBox() const
{
	return m_worldBox;
}

CU_NS_END
