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

// �ⲿͳһʹ�ô�����
#define PLUGIN_MAIN_NAME "PluginMain"
// ��CPP���Զ�����
#define DEF_PLUGIN(API, NewPlugin)\
	extern "C" API Plugin* PluginMain() { return new NewPlugin(); }

CUTE_NS_END