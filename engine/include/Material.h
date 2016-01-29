#pragma once
#include "Shader.h"
#include "Stream.h"

CU_NS_BEGIN

// 材质：包含Shader和参数
class CU_API Material : public Object
{
public:
	Material(Shader* shader);
	~Material();

	bool load(Stream* stream);
	void save(Stream* stream);

private:
	// todo:绑定的参数
	Shader*		m_shader;
	Technique*	m_tech;		// 当前使用的
};

typedef SharedPtr<Material> MaterialPtr;

CU_NS_END
