#pragma once
#include "D3D11_API.h"
#include <map>

CU_NS_BEGIN

class CU_D3D11_API D3D11Program : public Program
{
public:
	D3D11Program();
	~D3D11Program();

	void bind(ID3D11DeviceContextN* context);
	void unbind(ID3D11DeviceContextN* context);

private:
	ID3D11VertexShader*		_vs;
	ID3D11PixelShader*		_ps;
	ID3D11GeometryShader*	_gs;
	ID3D11DomainShader*		_ds;
	ID3D11HullShader*		_hs;
	ID3D11ComputeShader*	_cs;
};

CU_NS_END