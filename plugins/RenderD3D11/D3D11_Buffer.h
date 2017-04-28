#pragma once
#include "D3D11_API.h"

CUTE_NS_BEGIN

class D3D11_Device;
class CUTE_D3D11_API D3D11_Buffer : public GpuBuffer
{
public:
	D3D11_Buffer(const BufferDesc& desc, ID3D11DeviceN* device);
	~D3D11_Buffer();

	void* map(size_t offset, size_t len, MAP_FLAG access);
	void  unmap();

	ID3D11DepthStencilView* getDSV() { return (ID3D11DepthStencilView*)m_view; }

	ID3D11Buffer* native() { return m_handle; }

protected:
	ID3D11Buffer*	m_handle;
	ID3D11View*		m_view;
};

CUTE_NS_END
