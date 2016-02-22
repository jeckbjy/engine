#include "Component.h"
#include "SceneManager.h"

CU_NS_BEGIN

Component::Component()
{

}

Component::~Component()
{

}

void Component::attach(Entity* entity)
{
	gSceneMgr().attach(this);
	m_owner = entity;
}

void Component::detach()
{
	gSceneMgr().detach(this);
	m_owner = NULL;
}

CU_NS_END