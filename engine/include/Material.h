#pragma once
#include "Shader.h"
#include "Stream.h"

CU_NS_BEGIN

// ���ʣ�����Shader�Ͳ���,���pass��δ���??
class CU_API Material : public Object
{
public:
	Material(Shader* shader);
	~Material();

	bool load(Stream* stream);
	void save(Stream* stream);

	void setValue(const String& name, Texture* value, size_t index = 0);

	// ���õ���
	template<typename T>
	void setValue(const String& name, const T& value, size_t index = 0)
	{
		for (size_t i = 0; i < m_passes.size(); ++i)
		{
			DescriptorSet* descriptors = m_passes[i];
			descriptors->setValue(name, (void*)&value, sizeof(T), sizeof(T) * index);
		}
	}

	// ��ͨ���ݣ�����
	template<typename T>
	void setValue(const String& name, const T* value, size_t count = 1, size_t offset = 0)
	{
		for (size_t i = 0; i < m_passes.size(); ++i)
		{
			DescriptorSet* descriptors = m_passes[i];
			descriptors->setValue(name, (void*)value, sizeof(T) * count, sizeof(T)* offset);
		}
	}

private:
	// ÿ��passһ��
	typedef std::vector<DescriptorSet*> PassDescriptorVec;
	Shader*				m_shader;
	Technique*			m_tech;			// ��ǰʹ�õ�
	PassDescriptorVec	m_passes;
};

typedef SharedPtr<Material> MaterialPtr;

CU_NS_END
