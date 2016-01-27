#pragma once
#include "D3D11_Device.h"
#include "D3D11_Buffer.h"

CU_NS_BEGIN

D3D11Device::D3D11Device()
{
	D3D_FEATURE_LEVEL featureLevels[6] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1 };
	D3D_FEATURE_LEVEL cur_level;
	D3D11_CHECK(
		D3D11CreateDevice(NULL,
		D3D_DRIVER_TYPE_HARDWARE, 0, 0,
		featureLevels, 6, D3D11_SDK_VERSION,
		&m_device, &cur_level, &m_context),
		"D3D11CreateDevice fail!");
	m_factory = NULL;
	D3D11_CHECK(
		CreateDXGIFactory(__uuidof(IDXGIFactoryN), (void**)&m_factory),
		"CreateDXGIFactory fail!");
}

D3D11Device::~D3D11Device()
{
	D3D11_RELEASE(m_device);
}

GpuBuffer* D3D11Device::newBuffer(const BUFFER_DESC& desc)
{
	return new D3D11Buffer(desc, this);
}

CU_NS_END