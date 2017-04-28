#pragma once

// define CUTE_D3D11_API
#if defined(CUTE_USE_LIB_D3D11)
// build or use lib
#	define CUTE_D3D11_API
#elif defined(CUTE_BUILD_D3D11)
// build dynamic dll
#	define CUTE_D3D11_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_D3D11_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_D3D11) && !defined(CUTE_UNAUTOLINK_D3D11)
#	pragma comment(lib, "plugin_d3d11.lib")
#endif

//#ifdef _MSC_VER
//#pragma warning (disable : 4005)	// 宏冲定义
//#endif

#define D3D11_VERSION_0 0
#define D3D11_VERSION_1 1

#include "API.h"
#include "Graphics.h"
#ifdef CU_OS_WINRT
#include <d3d11_1.h>
#define D3D11_VERSION D3D11_VERSION_1
#else
#include <d3d11.h>
#define D3D11_VERSION D3D11_VERSION_0
#endif

#include <D3D11Shader.h>
#include <D3Dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define D3D11_RELEASE(x)		if(x != NULL) { x->Release(); x = NULL; }
#define D3D11_CHECK(hr, info)	if(FAILED(hr)) throw std::runtime_error(info);

#if (D3D11_VERSION == D3D11_VERSION_0)
// 在d3d11.1中才有logic_op
enum D3D11_LOGIC_OP
{
	D3D11_LOGIC_OP_CLEAR = 0,
	D3D11_LOGIC_OP_SET = (D3D11_LOGIC_OP_CLEAR + 1),
	D3D11_LOGIC_OP_COPY = (D3D11_LOGIC_OP_SET + 1),
	D3D11_LOGIC_OP_COPY_INVERTED = (D3D11_LOGIC_OP_COPY + 1),
	D3D11_LOGIC_OP_NOOP = (D3D11_LOGIC_OP_COPY_INVERTED + 1),
	D3D11_LOGIC_OP_INVERT = (D3D11_LOGIC_OP_NOOP + 1),
	D3D11_LOGIC_OP_AND = (D3D11_LOGIC_OP_INVERT + 1),
	D3D11_LOGIC_OP_NAND = (D3D11_LOGIC_OP_AND + 1),
	D3D11_LOGIC_OP_OR = (D3D11_LOGIC_OP_NAND + 1),
	D3D11_LOGIC_OP_NOR = (D3D11_LOGIC_OP_OR + 1),
	D3D11_LOGIC_OP_XOR = (D3D11_LOGIC_OP_NOR + 1),
	D3D11_LOGIC_OP_EQUIV = (D3D11_LOGIC_OP_XOR + 1),
	D3D11_LOGIC_OP_AND_REVERSE = (D3D11_LOGIC_OP_EQUIV + 1),
	D3D11_LOGIC_OP_AND_INVERTED = (D3D11_LOGIC_OP_AND_REVERSE + 1),
	D3D11_LOGIC_OP_OR_REVERSE = (D3D11_LOGIC_OP_AND_INVERTED + 1),
	D3D11_LOGIC_OP_OR_INVERTED = (D3D11_LOGIC_OP_OR_REVERSE + 1)
};

// 有些版本不支持
//enum D3D11_FILTER_REDUCTION_TYPE
//{
//	D3D11_FILTER_REDUCTION_TYPE_STANDARD = 0,
//	D3D11_FILTER_REDUCTION_TYPE_COMPARISON = 1,
//	D3D11_FILTER_REDUCTION_TYPE_MINIMUM = 2,
//	D3D11_FILTER_REDUCTION_TYPE_MAXIMUM = 3
//};

#endif

CUTE_NS_BEGIN

//#ifdef CU_OS_WINRT
////////////////////////////////////////////////////////////////////////////
//typedef ID3D11Device1           ID3D11DeviceN;
//typedef ID3D11DeviceContext1    ID3D11ContextN;
//typedef ID3D11RasterizerState1  ID3D11RasterizerStateN;
//typedef IDXGIFactory2           IDXGIFactoryN;
//typedef IDXGIAdapter1           IDXGIAdapterN;          // we don`t need IDXGIAdapter2 functionality
//typedef IDXGIDevice2            IDXGIDeviceN;
//typedef IDXGISwapChain1         IDXGISwapChainN;
//typedef DXGI_SWAP_CHAIN_DESC1   DXGI_SWAP_CHAIN_DESC_N;
//
//#else
////////////////////////////////////////////////////////////////////////////
//typedef ID3D11Device			ID3D11DeviceN;
//typedef ID3D11DeviceContext		ID3D11ContextN;
//typedef ID3D11RasterizerState	ID3D11RasterizerStateN;
//typedef IDXGIFactory1			IDXGIFactoryN;
//typedef IDXGIAdapter1			IDXGIAdapterN;
//typedef IDXGIDevice1            IDXGIDeviceN;
//typedef IDXGISwapChain          IDXGISwapChainN;
//typedef DXGI_SWAP_CHAIN_DESC    DXGI_SWAP_CHAIN_DESC_N;
//
//#endif

typedef ID3D11Device			ID3D11DeviceN;
typedef ID3D11DeviceContext		ID3D11ContextN;
typedef ID3D11RasterizerState	ID3D11RasterizerStateN;
typedef IDXGIFactory			IDXGIFactoryN;
typedef IDXGIAdapter			IDXGIAdapterN;
typedef IDXGIDevice				IDXGIDeviceN;
typedef IDXGISwapChain          IDXGISwapChainN;
typedef DXGI_SWAP_CHAIN_DESC    DXGI_SWAP_CHAIN_DESC_N;

enum D3D11ObjectID
{
	//OBJ_ID_D3D11_START = OBJ_ID_PLUGIN_D3D11,
	OBJ_ID_D3D11_FRAMEBUFFER,
	OBJ_ID_D3D11_SWAPCHAIN,
};

CUTE_NS_END
