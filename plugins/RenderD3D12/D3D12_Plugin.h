#pragma once
#include "Plugin.h"

CUTE_NS_BEGIN

#define D3D12_NAME "D3D12"
// ²å¼þÈë¿Ú
class D3D12_Plugin : public Plugin
{
public:
	D3D12_Plugin() : Plugin(D3D12_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
