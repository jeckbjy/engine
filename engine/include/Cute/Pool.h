#pragma once
#include "Cute/Mutex.h"
#include "Cute/List.h"

CUTE_NS_BEGIN

template<class C>
class Pool
{
public:
	Pool();
	~Pool();


private:
	typedef List<C> ObjectList;

	size_t		m_capacity;
	size_t		m_size;
	ObjectList	m_objects;
	Mutex		m_mutex;
};


CUTE_NS_END
