#pragma once
#include "API.h"

CU_NS_BEGIN

// dll��̬���ӿ�
class CU_API DynLib
{
	CU_NONCOPYABLE(DynLib);
public:
	typedef void(*DllFun)(void);
	static String& suffix();// ��׺
	static String check_path(const String& path);
public:
	DynLib();
	~DynLib();

	bool load(const String& path);
	void unload();
	void* getSymbol(const String& name);
	bool isLoaded() const;

private:
	module_t _handle;
};

CU_NS_END