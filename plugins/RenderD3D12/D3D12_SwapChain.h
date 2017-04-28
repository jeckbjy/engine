#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_SwapChain : public SwapChain
{
public:
	D3D12_SwapChain(const SwapChainDesc& desc);
	~D3D12_SwapChain();

	void present();
	void bind(void* param);

private:
	Window*				m_wnd;
	IDXGISwapChain*		m_chain;
};

CUTE_NS_END
