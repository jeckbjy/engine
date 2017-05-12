#pragma once
#include "D3D12_API.h"
#include "Cute/DynamicArray.h"

CUTE_NS_BEGIN

class D3D12_Device;
class D3D12_Texture;
class CUTE_D3D12_API D3D12_SwapChain : public ISwapChain
{
public:
	D3D12_SwapChain();
	~D3D12_SwapChain();

	bool init(D3D12_Device* device, const SwapChainDesc& desc);
	void term();

	void present();

private:
	typedef DynamicArray<D3D12_Texture*> TextureArray;
	//Window*				m_wnd;
	//IDXGISwapChain*		m_chain;
	IDXGISwapChain2*		m_chain;
	TextureArray			m_buffers;
	UINT					m_syncInterval;
	UINT					m_presentFlag;
	HWND					m_wnd;
};

CUTE_NS_END
