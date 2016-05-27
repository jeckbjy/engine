#pragma once
#include "Plugin.h"

CU_NS_BEGIN

#define D3D11_NAME "D3D11"
// ²å¼þÈë¿Ú
class D3D11_Plugin : public Plugin
{
public:
	D3D11_Plugin():Plugin(D3D11_NAME){}

	void install();
	void uninstall();
};

CU_NS_END