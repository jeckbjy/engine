#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class D3D11Texture;
class CU_D3D11_API D3D11RenderWindow : public D3D11RenderTarget
{
public:
	D3D11RenderWindow(Window* wnd, IDXGIFactoryN* factory, ID3D11DeviceN* device);
	~D3D11RenderWindow();

	void bind(ID3D11ContextN* context);

	void create(ID3D11DeviceN* device);
	void resize(uint32_t width, uint32_t height);

protected:
	Window*					m_wnd;
	IDXGISwapChainN*		m_chain;
	ID3D11Texture2D*		m_buffer;
	ID3D11RenderTargetView* m_rtv;
	D3D11Texture*			m_ds;
};

CU_NS_END