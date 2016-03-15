#include "D3D11_Buffer.h"
#include "D3D11_Device.h"

CU_NS_BEGIN

D3D11Buffer::D3D11Buffer(const BufferDesc& desc, D3D11Device* device)
:GpuBuffer(desc), m_device(device),m_handle(NULL), m_view(NULL)
{
	D3D11_BUFFER_DESC d3d_desc;
	d3d_desc.ByteWidth = m_bytes;
	d3d_desc.MiscFlags = 0;
	d3d_desc.StructureByteStride = 0;

	// todo:check system mem and random gpu
	// normal mode
	d3d_desc.Usage = isDynamic() ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	d3d_desc.CPUAccessFlags = isDynamic() ? D3D11_CPU_ACCESS_WRITE : 0;
	//switch (desc.type)
	//{
	//case BIND_VERTEX_BUFFER:
	//	d3d_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	break;
	//}
	D3D11_SUBRESOURCE_DATA init_data;
	init_data.SysMemPitch = 0;
	init_data.SysMemSlicePitch = 0;
	init_data.pSysMem = desc.data;
	D3D11_CHECK(
		device->getDevice()->CreateBuffer(&d3d_desc, &init_data, &m_handle),
		"CreateBuffer fail!"
		);
}

D3D11Buffer::~D3D11Buffer()
{
	D3D11_RELEASE(m_handle);
	D3D11_RELEASE(m_view);
}

void* D3D11Buffer::map(MAP_FLAG access, size_t offset, size_t len)
{
	return NULL;
}

void D3D11Buffer::unmap()
{

}

CU_NS_END