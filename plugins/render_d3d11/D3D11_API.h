#pragma once

#ifndef CU_D3D11_LIB
#define CU_D3D11_DLL
#endif

#if defined(CU_D3D11_LIB)
#	define CU_D3D11_API
#elif defined(CU_D3D11_BUILD)
#	define CU_D3D11_API CU_DECL_EXPORT
#else
#	define CU_D3D11_API CU_DECL_IMPORT
#endif

#if defined(_MSC_VER) && !defined(CU_D3D11_BUILD) && !defined(CU_D3D11_UN_AUTOLINK)
#pragma comment(lib, "plugin_d3d11.lib")
#endif

#ifdef _MSC_VER
#pragma warning (disable : 4005)	// ∫Í≥Â∂®“Â
#endif

#include "API.h"
#include "Graphics.h"
#ifdef CU_OS_WINRT
#include <d3d11_1.h>
#else
#include <d3d11.h>
#endif

#include <D3D11Shader.h>
#include <D3Dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")

#define D3D11_RELEASE(x)		if(x != NULL) { x->Release(); x = NULL; }
#define D3D11_CHECK(hr, info) if(FAILED(hr)) throw std::runtime_error(info);

CU_NS_BEGIN

#ifdef CU_OS_WINRT
//////////////////////////////////////////////////////////////////////////
typedef ID3D11Device1           ID3D11DeviceN;
typedef ID3D11DeviceContext1    ID3D11DeviceContextN;
typedef ID3D11RasterizerState1  ID3D11RasterizerStateN;
typedef IDXGIFactory2           IDXGIFactoryN;
typedef IDXGIAdapter1           IDXGIAdapterN;          // we don`t need IDXGIAdapter2 functionality
typedef IDXGIDevice2            IDXGIDeviceN;
typedef IDXGISwapChain1         IDXGISwapChainN;
typedef DXGI_SWAP_CHAIN_DESC1   DXGI_SWAP_CHAIN_DESC_N;

#else
//////////////////////////////////////////////////////////////////////////
typedef ID3D11Device			ID3D11DeviceN;
typedef ID3D11DeviceContext		ID3D11DeviceContextN;
typedef ID3D11RasterizerState	ID3D11RasterizerStateN;
typedef IDXGIFactory1			IDXGIFactoryN;
typedef IDXGIAdapter1			IDXGIAdapterN;
typedef IDXGIDevice1            IDXGIDeviceN;
typedef IDXGISwapChain          IDXGISwapChainN;
typedef DXGI_SWAP_CHAIN_DESC    DXGI_SWAP_CHAIN_DESC_N;

#endif

class CU_D3D11_API D3D11RenderTarget : public RenderTarget
{
public:
	virtual void bind(ID3D11DeviceContextN* context) = 0;
};

CU_NS_END
