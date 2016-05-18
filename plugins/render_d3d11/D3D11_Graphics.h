#pragma once
#include "D3D11_API.h"
#include "D3D11_Device.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11Graphics : public Graphics
{
public:
	D3D11Graphics();
	~D3D11Graphics();

	Device* getDevice() { return m_device; }

private:
	// �����hash����??
	D3D11Device* m_device;
};

CU_NS_END
