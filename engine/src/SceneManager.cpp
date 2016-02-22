#include "SceneManager.h"
#include "Camera.h"
#include "Drawable.h"

CU_NS_BEGIN

SceneManager::SceneManager()
{
	m_root = new Transform();
}

SceneManager::~SceneManager()
{
	delete m_root;
}

void SceneManager::clear(bool force /* = false */)
{
	// �����ӽڵ�
}

void SceneManager::update()
{
	// ����������
}

void SceneManager::attach(Component* comp)
{
	if (comp->isKindOf<Camera>())
	{
		m_cameras.push_back((Camera*)comp);
	}
	else if (comp->isKindOf<Drawable>())
	{
		m_drawables.push_back((Drawable*)comp);
	}
}

void SceneManager::detach(Component* comp)
{
	if (comp->isKindOf<Camera>())
	{
		m_cameras.erase((Camera*)comp);
	}
	else if (comp->isKindOf<Drawable>())
	{
		m_drawables.erase((Drawable*)comp);
	}
}

SceneManager& gSceneMgr()
{
	return SceneManager::Instance();
}

CU_NS_END
