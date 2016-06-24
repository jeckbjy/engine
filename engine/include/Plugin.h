#pragma once
#include "Ref.h"

CU_NS_BEGIN

/**
插件系统:修改自ogre
example:
class OpenGLPlugin : public Plugin {};

extern "C" Plugin* CU_API DllPluginMain(){ return new OpenGLPlugin(); }
*/
class CU_API Plugin
{
public:
	// 外部统一函数接口
	typedef Plugin* (*DLL_MAIN)(void);

public:
	Plugin(const String& pname) :m_name(pname){}
	virtual ~Plugin(){}

	virtual void install() = 0;
	virtual void uninstall() = 0;
	virtual void release() { delete this; }
	const String& name(){ return m_name; }
protected:
	String m_name;
};

typedef Plugin* (*PluginMainFun)(void);

// 外部统一使用此名字
#define DLL_PLUGIN_MAIN_NAME "DllPluginMain"
// 在CPP中自动定义
#define DEF_PLUGIN(API, PLUGIN) \
	extern "C" API Plugin* DllPluginMain() { return new PLUGIN(); }

CU_NS_END
