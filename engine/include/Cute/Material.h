#pragma once
#include "Cute/Foundation.h"
#include "Cute/Shader.h"
#include "Cute/Vector2.h"
#include "Cute/Vector3.h"
#include "Cute/Vector4.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Material
{
public:
    Material();
    ~Material();
    
    void setBuffer (const String& name, IBuffer* value);
    void setTexture(const String& name, ITexture* value);
    void setVector2(const String& name, const Vector2& value);
    void setVector3(const String& name, const Vector3& value);
    void setVector4(const String& name, const Vector4& value);
    
private:
    Shader* m_shader;
};

CUTE_NS_END
