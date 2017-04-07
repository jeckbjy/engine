#include "Cute/WeakPtr.h"

CUTE_NS_BEGIN

WeakRef::WeakRef()
{
}

WeakRef::WeakRef(const Flag* flag)
	: m_flag(flag)
{
}

bool WeakRef::isValid() const
{
	return !m_flag.isNull() && m_flag->isValid();
}

void WeakRef::reset()
{
	m_flag = NULL;
}

WeakRefOwner::WeakRefOwner()
{
}

WeakRefOwner::~WeakRefOwner()
{
	invalidate();
}

WeakRef WeakRefOwner::getRef() const
{
	if (m_flag.isNull())
		m_flag = new WeakRef::Flag();

	return WeakRef(m_flag.get());
}

bool WeakRefOwner::hasRefs() const
{
	return !m_flag.isNull();
}

void WeakRefOwner::invalidate()
{
	if (!m_flag.isNull())
	{
		m_flag->invalidate();
		m_flag = NULL;
	}
}

WeakPtrBase::WeakPtrBase()
{
}

WeakPtrBase::~WeakPtrBase()
{
}

WeakPtrBase::WeakPtrBase(const WeakRef& ref)
	: m_ref(ref)
{
}

CUTE_NS_END