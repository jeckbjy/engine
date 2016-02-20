#include "Drawable.h"

CU_NS_BEGIN

Drawable::Drawable()
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
