#pragma once
#include "StringPiece.h"
#include "Event.h"
#include "Plugin.h"
#include "DynLib.h"
#include "Singleton.h"

CU_NS_BEGIN

class Object;
class LibInfo;
// 全局上下文,FileSystem, ResourceCache
class CU_API Context : public Singleton<Context>
{
public:
	Context();
	~Context();

public:
	void addListener(EventHandler* handler);
	void removeListener(EventHandler* handler);
	void removeListener(Object* sender);
	void dispatch(Event* ev);
	
	void loadPlugin(const String& file);
	void unloadPlugin(const String& name);
	void unloadAllPlugin();
	Plugin* getPlugin(const String& name);

private:
	typedef List<EventHandler, 2> HandlerList;
	typedef std::map<uint32_t, HandlerList> HandlerMap;		// event->handlers
	typedef std::map<Object*, HandlerMap>	ObjectMap;		// object->handlers
	
	typedef std::map<String, LibInfo*>	LibMap;
	typedef std::map<String, LibInfo*>	PluginMap;

	HandlerMap	m_handlers;
	ObjectMap	m_senders;

	LibMap		m_libs;
	PluginMap	m_plugins;
};

CU_NS_END