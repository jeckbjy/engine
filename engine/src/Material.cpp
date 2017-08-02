//! Render
#include "Cute/Material.h"

CUTE_NS_BEGIN

Material::Material()
    : m_shader(NULL)
{
    
}

Material::~Material()
{
    
}

void Material::setShader(Shader *shader)
{
    m_shader = shader;
}

CUTE_NS_END

