#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class D3D11_Program;
class CU_D3D11_API D3D11_DescriptorSet : public DescriptorSet
{
public:
	D3D11_DescriptorSet(D3D11_Program* pipeline);
	~D3D11_DescriptorSet();

	void setValue(const String& name, Texture* texture, size_t index);
	void setValue(const String& name, void* data, size_t size, size_t offset);

	void bind(ID3D11ContextN* context);

private:
	typedef Object Descirptor;
	typedef std::vector<Descirptor*> DescriptorVec;
	DescriptorVec			m_descriptor;
	D3D11_Program*	m_pipeline;
};

CU_NS_END