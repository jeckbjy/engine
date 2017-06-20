#include "Cute/RTTI.h"

CUTE_NS_BEGIN

struct RttiManager
{
	typedef std::map<uint32_t, RTTI*>	TypeMap;
	typedef std::map<String, RTTI*>		NameMap;

	TypeMap typeMap;
	NameMap	nameMap;

	void regist(RTTI* rtti)
	{
		if (!rtti->getName().empty())
			nameMap[rtti->getName()] = rtti;
		
		if (rtti->getType() != 0)
			typeMap[rtti->getType()] = rtti;
	}

	RTTI* find(uint32_t type)
	{
		TypeMap::iterator itor = typeMap.find(type);
		if (itor != typeMap.end())
			return itor->second;

		return NULL;
	}

	RTTI* find(const String& name)
	{
		NameMap::iterator itor = nameMap.find(name);
		if (itor != nameMap.end())
			return itor->second;

		return NULL;
	}
};

static RttiManager gRttiMgr;

//////////////////////////////////////////////////////////////////////////
// RTTI
//////////////////////////////////////////////////////////////////////////
RTTI::RTTI(const RTTI* parent, const char* name, FourCC type, Creator fun)
	: m_parent(parent)
	, m_name(name)
	, m_type(type)
	, m_creator(fun)
{
	m_depth = parent ? parent->m_depth + 1 : 0;
	gRttiMgr.regist(this);
}

bool RTTI::isKindOf(const RTTI* other) const
{
	if (other == this)
		return true;
	// 自身深度必须大
	if (m_depth <= other->m_depth)
		return false;
	// 父类中，查找深度,必然能查到
	const RTTI* cur = m_parent;
	while (cur->m_depth != other->m_depth)
		cur = cur->m_parent;
	return cur == other;
}

bool RTTI::isKindOf(const String& name) const
{
	const RTTI* cur = this;
	while (cur && cur->m_name != name)
		cur = cur->m_parent;
	return cur != NULL;
}

bool RTTI::isKindOf(uint32_t type) const
{
	const RTTI* cur = this;
	while (cur && cur->m_type != type)
		cur = cur->m_parent;
	return cur != NULL;

}

void* RTTI::create()
{
	if (m_creator)
		return m_creator();

	return NULL;
}

CUTE_NS_END
