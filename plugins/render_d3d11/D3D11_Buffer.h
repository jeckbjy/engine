#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class D3D11Device;
class CU_D3D11_API D3D11Buffer : public GpuBuffer
{
public:
	D3D11Buffer(const BUFFER_DESC& desc, D3D11Device* device);
	~D3D11Buffer();

	void* map(MAP_FLAG access, size_t offset, size_t len);
	void  unmap();

	ID3D11DepthStencilView* getDSV() { return (ID3D11DepthStencilView*)m_view; }

protected:
	D3D11Device*	m_device;
	ID3D11Buffer*	m_handle;
	ID3D11View*		m_view;
};

CU_NS_END