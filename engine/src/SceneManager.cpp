#include "SceneManager.h"

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
	// 遍历子节点
}

void SceneManager::update()
{
	// 遍历整棵树
}

CU_NS_END
