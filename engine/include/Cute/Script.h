//! Script
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Script
{
public:
	Script();
	virtual ~Script();

	virtual void setup() = 0;
	virtual void destory() = 0;
	virtual void load(const String& path) = 0;

	// register function
	//virtual void regist(const String& name);
	//virtual void call(const String& name, std::vector<Any>& params) = 0;
};

CUTE_NS_END
