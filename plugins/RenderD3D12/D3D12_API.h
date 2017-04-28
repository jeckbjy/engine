#pragma once

// define CUTE_D3D12_API
#if defined(CUTE_USE_LIB_D3D12)
// build or use lib
#	define CUTE_D3D12_API
#elif defined(CUTE_BUILD_D3D12)
// build dynamic dll
#	define CUTE_D3D12_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_D3D12_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_D3D12) && !defined(CUTE_UNAUTOLINK_D3D12)
#	pragma comment(lib, "plugin_d3d12.lib")
#endif

#include "API.h"
#include "Graphics.h"
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3dcompiler.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define D3D12_RELEASE(handle) if(handle != NULL){ handle->Release(); handle = NULL; }
#define D3D12_CHECK(hr, info) if(FAILED(hr)) throw std::runtime_error(info)

// predeclare
CUTE_NS_BEGIN

class D3D12_Device;

CUTE_NS_END
