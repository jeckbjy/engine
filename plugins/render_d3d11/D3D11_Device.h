#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11Device : public Device
{
public:
	D3D11Device();
	~D3D11Device();

	GpuBuffer* newBuffer(const BufferDesc& desc);

	ID3D11DeviceN* getDevice() { return m_device; }

private:
	ID3D11DeviceN* m_device;
	IDXGIFactoryN* m_factory;
	ID3D11DeviceContextN* m_context;
};

CU_NS_END