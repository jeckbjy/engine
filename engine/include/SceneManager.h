#pragma once
#include "Transform.h"
#include "Singleton.h"

CU_NS_BEGIN

// ������,��������Object,ֻ����һ���󳡾�,���������Ϊ���ӽڵ�
class CU_API SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	virtual ~SceneManager();

	void clear(bool force = false);
	void update();

	Transform* getRoot() const { return m_root; }

protected:
	Transform* m_root;
};

CU_NS_END
