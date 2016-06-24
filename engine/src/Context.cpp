#include "Context.h"
#include "Object.h"

CU_NS_BEGIN

class LibInfo
{
public:
	DynLib* lib;
	Plugin* plugin;
	String	path;
	~LibInfo() { plugin->release();  delete lib; }
};

Context::Context()
{

}

Context::~Context()
{
	unloadAllPlugin();
}

void Context::addListener(EventHandler* handler)
{
	assert(handler->getSender() != handler->getObject());
	handler->retain();
	HandlerMap* handlers;
	if (handler->getSender() == NULL)
		handlers = &m_handlers;
	else
		handlers = &m_senders[handler->getSender()];
	(*handlers).at(handler->getType()).append(handler);
}

void Context::removeListener(EventHandler* handler)
{
	HandlerMap* handlers = NULL;
	Object* sender = handler->getSender();
	if (!sender)
	{
		// 没找到说明已经被释放了
		ObjectMap::iterator itor = m_senders.find(sender);
		if (itor == m_senders.end())
			return;
		handlers = &(itor->second);
	}
	else
		handlers = &m_handlers;
	(*handlers).at(handler->getType()).erase(handler);
	handler->release();
}

void Context::removeListener(Object* sender)
{
	// 删除所有
	ObjectMap::iterator itor = m_senders.find(sender);
	if (itor == m_senders.end())
		return;
	// 释放所有
	HandlerMap& handlers = itor->second;
	for (HandlerMap::iterator itor1 = handlers.begin(); itor1 != handlers.end(); ++itor1)
	{
		HandlerList& handlers = itor1->second;
		if (handlers.empty())
			continue;
		EventHandler* handler;
		HandlerList::iterator itor2 = handlers.begin();
		HandlerList::iterator endIter = handlers.end();
		HandlerList::iterator tmp;
		while (itor2 != endIter)
		{
			handler = *itor2;
			tmp = itor2++;
			handlers.erase(tmp);
			handler->getObject()->removeListener(handler);
			handler->release();
		}
	}
	m_senders.erase(itor);
}

void Context::loadPlugin(const String& file)
{
	String path = DynLib::checkPath(file);
	// 防止重复加载
	if (m_libs.find(path) != m_libs.end())
		return;
	// todo:先校验文件是否存在
	DynLib* lib = new DynLib();
	if (!lib->load(path))
	{
		delete lib;
		return;
	}
	Plugin* plugin = NULL;
	Plugin::DLL_MAIN main_fun = (Plugin::DLL_MAIN)lib->getSymbol(DLL_PLUGIN_MAIN_NAME);
	if (main_fun)
		plugin = main_fun();
	assert(plugin != NULL);
	plugin->install();
	PluginMap::iterator itor = m_plugins.find(plugin->name());
	assert(itor == m_plugins.end());
	LibInfo* info = new LibInfo();
	info->path = path;
	info->lib = lib;
	info->plugin = plugin;
	m_plugins[plugin->name()] = info;
	m_libs[path] = info;
}

void Context::unloadPlugin(const String& name)
{
	// 先查找plugin，没找到再查找lib
	LibInfo* info = NULL;
	PluginMap::iterator pluginItor = m_plugins.find(name);
	LibMap::iterator libItor;
	if (pluginItor != m_plugins.end())
	{
		info = pluginItor->second;
		libItor = m_libs.find(info->path);
	}
	else
	{
		libItor = m_libs.find(name);
		if (libItor == m_libs.end())
			return;
		info = libItor->second;
		pluginItor = m_plugins.find(info->plugin->name());
	}
	m_plugins.erase(pluginItor);
	m_libs.erase(libItor);
	delete info;
}

void Context::unloadAllPlugin()
{
	for (LibMap::iterator itor = m_libs.begin(); itor != m_libs.end(); ++itor)
	{
		delete itor->second;
	}
	m_libs.clear();
	m_plugins.clear();
}

Plugin* Context::getPlugin(const String& name)
{
	PluginMap::iterator itor = m_plugins.find(name);
	if (itor != m_plugins.end())
		return itor->second->plugin;
	return NULL;
}

CU_NS_END
