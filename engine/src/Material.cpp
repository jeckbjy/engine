#include "Material.h"

CU_NS_BEGIN

Material::Material(Shader* shader)
{

}

Material::~Material()
{

}

bool Material::load(Stream* stream)
{
	return true;
}

void Material::save(Stream* stream)
{

}

void Material::setValue(const String& name, Texture* value, size_t index /* = 0 */)
{
	for (size_t i = 0; i < m_passes.size(); ++i)
	{
		DescriptorSet* descriptors = m_passes[i];
		descriptors->setValue(name, value, index);
	}
}

CU_NS_END
