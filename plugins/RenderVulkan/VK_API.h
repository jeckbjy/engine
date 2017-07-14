#pragma once

// define CUTE_VK_API
#if defined(CUTE_USE_LIB_VALKAN)
// build or use lib
#	define CUTE_VK_API
#elif defined(CUTE_BUILD_VALKAN)
// build dynamic dll
#	define CUTE_VK_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_VK_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_VALKAN) && !defined(CUTE_UNAUTOLINK_VALKAN)
#	pragma comment(lib, "plugin_valkan.lib")
#endif

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#else
#define VK_USE_PLATFORM_XCB_KHR
#endif

#include "Cute/API.h"
#include "Cute/Graphics.h"
#include <vulkan/vulkan.h>

#define VK_CHECK(result, info) if(result != VK_SUCCESS) throw std::runtime_error(info)


