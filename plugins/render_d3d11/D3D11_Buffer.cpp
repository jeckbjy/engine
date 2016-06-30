#include "D3D11_Buffer.h"
#include "D3D11_Device.h"
#include "D3D11_Mapping.h"

CU_NS_BEGIN

UINT getBindFlags(BufferUsage usage)
{
	switch (usage)
	{
	case BU_VERTEX:
		return D3D11_BIND_VERTEX_BUFFER;
	case BU_INDEX:
		return D3D11_BIND_INDEX_BUFFER;
	case BU_UNIFORM:
		return D3D11_BIND_CONSTANT_BUFFER;
	default:
		return 0;
	}
}

D3D11_Buffer::D3D11_Buffer(const BufferDesc& desc, ID3D11DeviceN* device)
	: GpuBuffer(desc)
	, m_handle(NULL)
	, m_view(NULL)
{
	D3D11_BUFFER_DESC d3d_desc;
	d3d_desc.ByteWidth = m_bytes;
	d3d_desc.MiscFlags = 0;
	d3d_desc.StructureByteStride = 0;

	d3d_desc.Usage = isDynamic() ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	d3d_desc.CPUAccessFlags = isDynamic() ? D3D11_CPU_ACCESS_WRITE : 0;
	d3d_desc.BindFlags = getBindFlags(desc.usage);

	D3D11_SUBRESOURCE_DATA init_data;
	init_data.SysMemPitch = 0;
	init_data.SysMemSlicePitch = 0;
	init_data.pSysMem = desc.data;

	HRESULT hr = device->CreateBuffer(&d3d_desc, desc.data ? &init_data : NULL, &m_handle);
	D3D11_CHECK( hr, "CreateBuffer fail!");
}

D3D11_Buffer::~D3D11_Buffer()
{
	D3D11_RELEASE(m_handle);
	D3D11_RELEASE(m_view);
}

void* D3D11_Buffer::map(size_t offset, size_t len, MAP_FLAG access)
{
	ID3D11ContextN* context = gD3D11NativeContext();
	if (context)
	{
		D3D11_MAP type = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE res;
		context->Map(m_handle, 0, type, 0,  &res);
		return (char*)res.pData + offset;
	}
	return NULL;
}

void D3D11_Buffer::unmap()
{
	ID3D11ContextN* context = gD3D11NativeContext();
	if (context)
		context->Unmap(m_handle, 0);
}

CU_NS_END