#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class D3D11Device;
class CU_D3D11_API D3D11Buffer : public GpuBuffer
{
public:
	D3D11Buffer(const BufferDesc& desc, ID3D11DeviceN* device);
	~D3D11Buffer();

	void* map(size_t offset, size_t len, MAP_FLAG access);
	void  unmap();

	ID3D11DepthStencilView* getDSV() { return (ID3D11DepthStencilView*)m_view; }

	ID3D11Buffer* native() { return m_handle; }

protected:
	ID3D11Buffer*	m_handle;
	ID3D11View*		m_view;
};

CU_NS_END