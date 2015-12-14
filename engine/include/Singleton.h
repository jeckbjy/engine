#pragma once
#include "API.h"

CU_NS_BEGIN

template<typename T>
class Singleton
{
public:
	static T& Instance()
	{
		static T _instance;
		return _instance;
	}
	static T* InstancePtr()
	{
		return &Instance();
	}

protected:
	Singleton(void){};
	virtual ~Singleton(){};

private:
	Singleton(const Singleton&);
	Singleton& operator =(const Singleton&);
};

//template<typename T>
//T Singleton<T>::m_instance;

CU_NS_END