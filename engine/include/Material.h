#pragma once
#include "Shader.h"

CU_NS_BEGIN

// ���ʣ�����Shader�Ͳ���
class CU_API Material : public Object
{
public:
	Material(Shader* shader);
	~Material();

private:
	// todo:�󶨵Ĳ���
	Shader*		m_shader;
	Technique*	m_tech;		// ��ǰʹ�õ�
};

CU_NS_END
