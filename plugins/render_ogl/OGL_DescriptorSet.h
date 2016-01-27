#pragma once
#include "OGL_API.h"
#include "OGL_Buffer.h"

CU_NS_BEGIN

typedef GpuResource Descriptor;
// ÃèÊö·û
class OGLProgram;
class CU_OGL_API OGLDescriptorSet : public DescriptorSet
{
	typedef std::vector<Descriptor*> DescriptorVec;
public:
	OGLDescriptorSet(Program* prog);
	~OGLDescriptorSet();

	void bind(const String& name, GpuResource* res);

	Descriptor* getDescriptor(size_t index);

	inline size_t size() const { return m_descriptors.size(); }
	inline Descriptor* at(size_t idx) { return m_descriptors[idx]; }

protected:
	OGLProgram*		m_prog;
	DescriptorVec	m_descriptors;
};

CU_NS_END