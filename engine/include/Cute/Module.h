#pragma once
#include "Cute/Foundation.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

template<typename T>
class Module
{
public:
	static T& instance()
	{
		T** holder = _instance();
		return **holder;
	}

	static T* instancePtr()
	{
		T** holder = _instance();
		return *holder;
	}

	static void startup(T* inst)
	{
		T** holder = _instance();
		if (*holder != NULL)
			throw RuntimeException("Trying to start an already started module.");

		*holder = inst;
	}

	static void shutdown()
	{
		T** holder = _instance();
		T*  inst = *holder;
		if (inst != NULL)
		{
			delete inst;
			*holder = NULL;
		}
	}

protected:
	Module()
	{
	}

	~Module()
	{
	}

	static T** _instance()
	{
		static T* inst = NULL;
		return &inst;
	}

private:
	Module(const Module&);
	Module& operator=(const Module&);
};

CUTE_NS_END
