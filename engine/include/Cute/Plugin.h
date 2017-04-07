#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Plugin
{
public:
	Plugin(const String& pluginName);
	virtual ~Plugin();

	virtual void install() = 0;
	virtual void uninstall() = 0;
	virtual void release();

	const String& name() const;

protected:
	String m_name;
};

inline const String& Plugin::name() const
{
	return m_name;
}

// 外部统一使用此名字
#define PLUGIN_MAIN_NAME "PluginMain"
// 在CPP中自动定义
#define DEF_PLUGIN(API, NewPlugin)\
	extern "C" API Plugin* PluginMain() { return new NewPlugin(); }

CUTE_NS_END