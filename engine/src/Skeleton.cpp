#include "Skeleton.h"

CU_NS_BEGIN

Skeleton::Skeleton()
{

}

Skeleton::~Skeleton()
{

}

const Bone* Skeleton::getBone(const String& name) const
{
	BoneMap::const_iterator itor = m_names.find(name);
	if (itor != m_names.end())
		return itor->second;
	return NULL;
}

CU_NS_END