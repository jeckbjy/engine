#pragma once
#include "D3D12_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define D3D12_NAME "D3D12"
// ²å¼þÈë¿Ú
class CUTE_D3D12_API D3D12_Plugin : public Plugin
{
public:
	D3D12_Plugin() : Plugin(D3D12_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
