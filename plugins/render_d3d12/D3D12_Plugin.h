#pragma once
#include "Plugin.h"

CU_NS_BEGIN

#define D3D12_NAME "D3D12"
// ������
class D3D12Plugin : public Plugin
{
public:
	D3D12Plugin() : Plugin(D3D12_NAME){}

	void install();
	void uninstall();
};

CU_NS_END