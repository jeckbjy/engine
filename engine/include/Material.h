#pragma once
#include "Shader.h"
#include "Stream.h"

CU_NS_BEGIN

// ���ʣ�����Shader�Ͳ���
class CU_API Material : public Object
{
public:
	Material(Shader* shader);
	~Material();

	bool load(Stream* stream);
	void save(Stream* stream);

private:
	// todo:�󶨵Ĳ���
	Shader*		m_shader;
	Technique*	m_tech;		// ��ǰʹ�õ�
};

typedef SharedPtr<Material> MaterialPtr;

CU_NS_END
