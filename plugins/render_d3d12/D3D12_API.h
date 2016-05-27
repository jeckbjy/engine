#pragma once

#ifndef CU_D3D12_LIB
#define CU_D3D12_DLL
#endif

#if defined(CU_D3D12_LIB)
#	define CU_D3D12_API
#elif defined(CU_D3D12_BUILD)
#	define CU_D3D12_API CU_DECL_EXPORT
#else
#	define CU_D3D12_API CU_DECL_IMPORT
#endif

#if defined(_MSC_VER) && !defined(CU_D3D12_BUILD) && !defined(CU_D3D12_UN_AUTOLINK)
#pragma comment(lib, "plugin_d3d12.lib")
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
CU_NS_BEGIN

class D3D12_Device;

CU_NS_END
