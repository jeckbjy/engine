#pragma once
#include "Transform.h"
#include "Singleton.h"

CU_NS_BEGIN

// 管理器,管理所有Object,只能有一个大场景,多个场景则为其子节点
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
