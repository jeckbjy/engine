#pragma once

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

#include <vulkan/vulkan.h>

#include "API.h"
#include "Graphics.h"

#define VK_CHECK(result, info) if(result != VK_SUCCESS) throw std::runtime_error(info)
