#include "Renderer.h"
#include "SceneManager.h"

CU_NS_BEGIN

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::update()
{
	// ���Ƴ���
	CameraList& cameras = gSceneMgr().getCameras();
	for (CameraList::iterator itor = cameras.begin(); itor != cameras.end(); ++itor)
	{
		// ����visiable
	}
}

CU_NS_END