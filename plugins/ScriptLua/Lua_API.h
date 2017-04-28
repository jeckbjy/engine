#pragma once

// define CUTE_LUA_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_LUA)
// build or use lib
#	define CUTE_LUA_API
#elif defined(CUTE_BUILD_LUA)
// build dynamic dll
#	define CUTE_LUA_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_LUA_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_LUA) && !defined(CUTE_UNAUTOLINK_LUA)
#	pragma comment(lib, "plugin_lua.lib")
#endif

#define LUA_PLUGIN_NAME "lua"
