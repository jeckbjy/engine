#pragma once
#include "Transform.h"
#include "Singleton.h"
#include "Octree.h"

CU_NS_BEGIN

// 管理器,管理所有Object,只能有一个大场景,多个场景则为其子节点
typedef List<Camera, 2>		CameraList;
typedef List<Drawable, 2>	DrawableList;
class CU_API SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	virtual ~SceneManager();

	void clear(bool force = false);
	void update();

	void attach(Component* comp);
	void detach(Component* comp);

	Transform* getRoot() const { return m_root; }
	CameraList& getCameras() { return m_cameras; }
	Octree* getOctree() { return &m_octree; }

protected:
	Transform*	m_root;
	Octree		m_octree;
	CameraList	m_cameras;
	DrawableList m_drawables;
};

CU_API SceneManager& gSceneMgr();

CU_NS_END
