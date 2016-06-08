#pragma once

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#else
#define VK_USE_PLATFORM_XCB_KHR
#endif

#ifndef CU_VULKAN_LIB
#define CU_VULKAN_DLL
#endif

#if defined(CU_VULKAN_LIB)
#	define CU_VK_API
#elif defined(CU_VULKAN_BUILD)
#	define CU_VK_API CU_DECL_EXPORT
#else
#	define CU_VK_API CU_DECL_IMPORT
#endif

#if defined(_MSC_VER) && !defined(CU_VULKAN_BUILD) && !defined(CU_VULKAN_UN_AUTOLINK)
#pragma comment(lib, "plugin_vulkan.lib")
#endif

#include "API.h"
#include "Graphics.h"
#include <vulkan/vulkan.h>

#define VK_CHECK(result, info) if(result != VK_SUCCESS) throw std::runtime_error(info)
