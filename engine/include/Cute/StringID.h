#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API StringID
{
public:
	StringID();
	StringID(const char* name);
	StringID(const char* name, size_t leng);
	StringID(const String& name);
	~StringID();

	bool operator ==(const StringID& rhs) const 
	{
		return m_data == rhs.m_data; 
	}

	bool operator !=(const StringID& rhs) const 
	{
		return m_data != rhs.m_data; 
	}

	bool empty() const { return m_data == NULL; }
	const char* str() const;
	size_t size() const;

private:
	void* m_data;
};

CUTE_NS_END
