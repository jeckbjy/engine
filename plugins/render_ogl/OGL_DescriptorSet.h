#pragma once
#include "OGL_API.h"
#include "OGL_Buffer.h"

CU_NS_BEGIN

class OGL_Pipeline;
class CU_OGL_API OGL_DescriptorSet : public DescriptorSet
{
public:
	OGL_DescriptorSet(Pipeline* pipeline);
	~OGL_DescriptorSet();

	void setValue(const String& name, Texture* texture, size_t index);
	void setValue(const String& name, void* data, size_t size, size_t offset);

protected:
	OGL_Pipeline* m_pipeline;
};

CU_NS_END