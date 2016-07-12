#include "OGL_DescriptorSet.h"
#include "OGL_Program.h"
#include "OGL_Pipeline.h"

CU_NS_BEGIN

OGL_DescriptorSet::OGL_DescriptorSet(OGL_Program* prog)
	: m_prog(prog)
{
}

OGL_DescriptorSet::~OGL_DescriptorSet()
{
}

void OGL_DescriptorSet::setValue(const String& name, Texture* texture, size_t index)
{

}

void OGL_DescriptorSet::setValue(const String& name, const void* data, size_t size, size_t offset)
{

}

void OGL_DescriptorSet::bind()
{
	const UniformVec& uniforms = m_prog->getUniformVec();
	if (uniforms.empty())
		return;
	for (size_t i = 0; i < uniforms.size(); ++i)
	{
		// 绑定每一个

	}
}

CU_NS_END