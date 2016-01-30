#include "Skeleton.h"
#include "Stream.h"

CU_NS_BEGIN

Skeleton::Skeleton()
{

}

Skeleton::~Skeleton()
{

}

bool Skeleton::load(Stream* stream)
{
	return true;
}

void Skeleton::save(Stream* stream)
{
	// shape?
	// write bone
	size_t count = m_bones.size();
	stream->writeVariant(count);
	for (size_t i = 0; i < count; ++i)
	{
		const Bone* bone = m_bones[i];
		stream->writeVariant(bone->index + 1);
		stream->writeVariant(bone->parent ? bone->parent->index + 1 : 0);
		stream->write(bone->name);
		stream->write(bone->pose.data(), bone->pose.size());
	}
}

const Bone* Skeleton::getBone(const String& name) const
{
	BoneMap::const_iterator itor = m_names.find(name);
	if (itor != m_names.end())
		return itor->second;
	return NULL;
}

CU_NS_END