#pragma once
#include "Lua_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

class CUTE_LUA_API LuaPlugin : public Plugin
{
public:
	LuaPlugin() : Plugin(LUA_PLUGIN_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END

