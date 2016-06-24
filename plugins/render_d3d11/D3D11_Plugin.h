#pragma once
#include "Plugin.h"

CU_NS_BEGIN

#define D3D11_NAME "D3D11"
// ²å¼þÈë¿Ú
class D3D11_Device;
class D3D11_Plugin : public Plugin
{
public:
	D3D11_Plugin();
	~D3D11_Plugin();

	void install();
	void uninstall();

private:
	D3D11_Device* m_device;
};

CU_NS_END