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
	// ªÊ÷∆≥°æ∞
	CameraList& cameras = gSceneMgr().getCameras();
	for (CameraList::iterator itor = cameras.begin(); itor != cameras.end(); ++itor)
	{
		// ≤È’“visiable
	}
}

CU_NS_END