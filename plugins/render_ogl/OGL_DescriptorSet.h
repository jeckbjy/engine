#pragma once
#include "OGL_API.h"
#include "OGL_Buffer.h"

CU_NS_BEGIN

class OGL_Program;
class CU_OGL_API OGL_DescriptorSet : public DescriptorSet
{
public:
	OGL_DescriptorSet(OGL_Program* prog);
	~OGL_DescriptorSet();

	void setValue(const String& name, Texture* texture, size_t index);
	void setValue(const String& name, const void* data, size_t size, size_t offset);

	void bind();

protected:
	enum Type
	{
		TYPE_NONE,
		TYPE_BUFFER,
		TYPE_TEXTURE,
		TYPE_TEXTURE_ARRAY,
	};

	struct Descriptor
	{
		Type type;
		union
		{
			OGL_Buffer*		buffer;
			OGL_Texture*	texture;
			OGL_Texture**	textureArray;
		};
	};
	typedef std::vector<Descriptor> DescriptorVec;
	OGL_Program*  m_prog;
	DescriptorVec m_descriptors;
};

CU_NS_END