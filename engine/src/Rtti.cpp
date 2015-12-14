#include "Rtti.h"

CU_NS_BEGIN

struct RttiManager
{
	typedef std::map<uint32_t, Rtti*> FourCCMap;
	typedef std::map<String, Rtti*> NameMap;

	FourCCMap fourCCMap;
	NameMap   nameMap;

	Rtti* find(uint32_t type)
	{
		FourCCMap::iterator itor = fourCCMap.find(type);
		if (itor != fourCCMap.end())
			return itor->second;
		return NULL;
	}

	Rtti* find(const String& name)
	{
		NameMap::iterator itor = nameMap.find(name);
		if (itor != nameMap.end())
			return itor->second;
		return NULL;
	}
};

static RttiManager gRttiMgr;

Rtti* Rtti::find(uint32_t type)
{
	return gRttiMgr.find(type);
}

Rtti* Rtti::find(const String& name)
{
	return gRttiMgr.find(name);
}

Rtti::Rtti(const Rtti* parent, const char* name, FourCC fourCC, Creator fun)
: m_parent(parent)
, m_name(name)
, m_type(fourCC)
, m_creator(fun)
, m_registed(false)
{
	m_depth = parent ? parent->m_depth + 1 : 0;
	if (name)
		gRttiMgr.nameMap[name] = this;
	if (fourCC)
		gRttiMgr.fourCCMap[fourCC] = this;
}

bool Rtti::isKindOf(const Rtti* other) const
{
	if (other == this)
		return true;
	// 自身深度必须大
	if (m_depth <= other->m_depth)
		return false;
	// 父类中，查找深度,必然能查到
	const Rtti* cur = m_parent;
	while (cur->m_depth != other->m_depth)
		cur = cur->m_parent;
	return cur == other;
}

bool Rtti::isKindOf(const String& name) const
{
	const Rtti* cur = this;
	while (cur && cur->m_name != name)
		cur = cur->m_parent;
	return cur != NULL;
}

bool Rtti::isKindOf(uint32_t type) const
{
	const Rtti* cur = this;
	while (cur && cur->m_type != type)
		cur = cur->m_parent;
	return cur != NULL;

}

void Rtti::addAttribute(const Attribute& attr)
{
	m_registed = true;
	m_attributes.push_back(attr);
}

CU_NS_END