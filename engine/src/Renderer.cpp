#include "Renderer.h"
#include "SceneManager.h"
#include "WorkQueue.h"
#include "Camera.h"

CU_NS_BEGIN

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::update()
{
	// 绘制场景
	CameraList& cameras = gSceneMgr().getCameras();
	for (CameraList::iterator itor = cameras.begin(); itor != cameras.end(); ++itor)
	{
		// 查找visiable
	}
}

void Renderer::render(Camera* camera)
{
	RenderPath* renderPath = camera->getRenderPath();
	for (size_t i = 0; i < renderPath->commands.size(); ++i)
	{
		RenderCommand& command = renderPath->commands[i];
		// 执行每一个command
		switch (command.type)
		{
		case CMD_CLEAR:
		{
			break;
		}
		case CMD_SCENE_PASS:
		{
			break;
		}
		case CMD_QUAD:
		{
			break;
		}
		case CMD_FORWARD_LIGHTS:
		{
			break;
		}
		case CMD_DEFER_LIGHTS:
		{
			break;
		}
		}
	}
}

CU_NS_END