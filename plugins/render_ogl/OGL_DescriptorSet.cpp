#include "OGL_DescriptorSet.h"
#include "OGL_Program.h"

CU_NS_BEGIN

OGLDescriptorSet::OGLDescriptorSet(Program* prog)
:m_prog((OGLProgram*)prog)
{
	size_t count = m_prog->uniformCount();
	if (count > 0)
		m_descriptors.resize(count);
}

OGLDescriptorSet::~OGLDescriptorSet()
{
	m_descriptors.clear();
}

void OGLDescriptorSet::bind(const String& name, GpuResource* res)
{
	const ParamInfo* info = m_prog->getInfo(name);
	if (!info)
		return;
	m_descriptors[info->index] = res;
}

Descriptor* OGLDescriptorSet::getDescriptor(size_t index)
{
	if (index >= m_descriptors.size())
		return NULL;
	return m_descriptors[index];
}

CU_NS_END