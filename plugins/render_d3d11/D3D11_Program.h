#pragma once
#include "D3D11_API.h"
#include <map>

CU_NS_BEGIN

class CU_D3D11_API D3D11Program : public Program
{
public:
	D3D11Program();
	~D3D11Program();

	bool compile(const ProgramDesc& desc);
	//void load();
	//void save();

	ID3DBlob* getCode() { return m_code; }
private:
	void create(ShaderStage stage, DWORD* code, SIZE_T size);

private:
	ID3D11DeviceChild*	m_shader;
	ID3DBlob*			m_code;		// vsÐèÒª
};

CU_NS_END