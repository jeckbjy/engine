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
	// �����ӽڵ�
}

void SceneManager::update()
{
	// ����������
}

CU_NS_END
