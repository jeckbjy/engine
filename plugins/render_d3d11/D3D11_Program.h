#pragma once
#include "D3D11_API.h"
#include <map>

CU_NS_BEGIN

class CU_D3D11_API D3D11Program : public Program
{
public:
	D3D11Program(uint32_t id);
	~D3D11Program();

	bool compile(const ProgramDesc& desc);
	//void load();
	//void save();

	uint32_t getID() const { return m_id; }
	ID3DBlob* getCode() { return m_code; }

	template<typename T>
	T* getShader() { return (T*)m_shader; }

private:
	void create(ShaderStage stage, DWORD* code, SIZE_T size);

private:
	ID3D11DeviceChild*	m_shader;
	ID3DBlob*			m_code;		// vs需要
	uint32_t			m_id;		// 唯一ID
	// 解析
};

CU_NS_END