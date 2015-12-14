#include "Ref.h"

CU_NS_BEGIN

RefCounted::RefCounted()
{
	m_ref = new RefCount(this);
}

RefCounted::~RefCounted()
{
	m_ref->release();
}

void RefCounted::retain()
{
	++m_ref->refs_count;
}

void RefCounted::release()
{
	if (--m_ref->refs_count <= 0)
		delete this;
}

CU_NS_END