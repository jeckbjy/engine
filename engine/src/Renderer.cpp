#include "Renderer.h"
#include "SceneManager.h"
#include "WorkQueue.h"
#include "View.h"
#include "RenderPath.h"

CU_NS_BEGIN

void UpdateViewWork(const WorkItem* item)
{
	View* view = (View*)item->data;
	view->update();
}

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::update()
{
	// 多线程更新View
	//for (size_t i = 0; i < m_views.size(); ++i)
	//{
	//	gWorkQueue().addItem(&UpdateViewWork, m_views[i], NULL, NULL);
	//}
	//gWorkQueue().execute();
	// 先单线程执行
	for (size_t i = 0; i < m_views.size(); ++i)
	{
		m_views[i]->update();
	}
	// 绘制每一个场景
	for (size_t i = 0; i < m_views.size(); ++i)
	{
		m_views[i]->render();
	}
}

Texture* Renderer::getGBuffer(size_t width, size_t height, size_t format, bool cubemap, bool filtered, bool srgb, unsigned persistent)
{
	if (PixelUtil::isDepthStencil((PixelFormat)format))
	{
		filtered = false;
		srgb = false;
	}
	if (cubemap)
		height = width;

	uint64 key = ((uint64)format << 32) | (width << 16) | height;
	if (filtered)
		key |= 0x8000000000000000LL;
	if (srgb)
		key |= 0x4000000000000000LL;
	if (cubemap)
		key |= 0x2000000000000000LL;
	if (persistent)
		key += ((uint64)persistent << 32);

	TextureMap::iterator itor = m_gbuffers.find(key);
	if (itor != m_gbuffers.end())
		return itor->second;
	TextureDesc desc;
	//desc.type = cubemap ? TEX_CUBE : TEX_2D;
	desc.type = TEX_2D;
	desc.width = width;
	desc.height = height;
	desc.format = (PixelFormat)format;
	Texture* texture = NULL;
	//Texture* texture = m_device->newTexture();
	// filter?
	m_gbuffers[key] = texture;
	return texture;
}

CU_NS_END
