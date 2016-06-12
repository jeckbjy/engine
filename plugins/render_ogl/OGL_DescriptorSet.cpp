#include "OGL_DescriptorSet.h"
#include "OGL_Program.h"
#include "OGL_Pipeline.h"

CU_NS_BEGIN

OGL_DescriptorSet::OGL_DescriptorSet(Pipeline* pipeline)
	: m_pipeline((OGL_Pipeline*)pipeline)
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

CU_NS_END