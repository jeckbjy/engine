#pragma once
#include "Plugin.h"

CU_NS_BEGIN

#define D3D11_NAME "D3D11"
// ������
class D3D11Plugin : public Plugin
{
public:
	D3D11Plugin():Plugin(D3D11_NAME){}

	void install();
	void uninstall();
};

CU_NS_END