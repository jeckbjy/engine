#pragma once
#include "Shader.h"

CU_NS_BEGIN

// 材质：包含Shader和参数
class CU_API Material : public Object
{
public:
	Material(Shader* shader);
	~Material();

private:
	// todo:绑定的参数
	Shader*		m_shader;
	Technique*	m_tech;		// 当前使用的
};

CU_NS_END
