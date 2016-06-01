#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class D3D11_Device;
class D3D11_Buffer;
class D3D11_Texture;
class D3D11_Program;
class CU_D3D11_API D3D11_DescriptorSet : public DescriptorSet
{
public:
	D3D11_DescriptorSet(D3D11_Program* program, D3D11_Device* device);
	~D3D11_DescriptorSet();

	void setValue(const String& name, Texture* texture, size_t index);
	void setValue(const String& name, const void* data, size_t size, size_t offset);

	void bind(ID3D11ContextN* context);

private:
	enum Type
	{
		TYPE_NONE,
		TYPE_BUFFER,
		TYPE_TEXTURE,
	};

	struct Descriptor
	{
		Type	type;
		union
		{
			void*			pointer;
			D3D11_Buffer*	buffer;
			D3D11_Texture**	textures;
		};
		Descriptor()
		{
			type = TYPE_NONE;
			pointer = NULL;
		}
		void set(Type type, void* pointer)
		{
			this->type = type;
			this->pointer = pointer;
		}
	};
	// Êý×éÖ¸Õë
	typedef std::vector<Descriptor> DescriptorVec;
	DescriptorVec	m_descriptors;
	D3D11_Program*	m_program;
};

CU_NS_END