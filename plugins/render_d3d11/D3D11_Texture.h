#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11Texture : public Texture
{
public:
	D3D11Texture(const TEXTURE_DESC& desc);
	~D3D11Texture();

	ID3D11DepthStencilView* getDSV() { return (ID3D11DepthStencilView*)_view; }
private:
	ID3D11Resource* _handle;
	ID3D11View*		_view;
};

CU_NS_END