#pragma once
#include "Object.h"
#include "Graphics.h"

CU_NS_BEGIN

// 高层渲染,全局唯一,TBR(tile-based render) or IMR()
/*
可以多线程执行,每个线程执行一个Camera，但是渲染需要安Camera顺序执行
渲染流程:
1:排序Camera
2:遍历所有Camera，对每个活动的Camera依次执行下面渲染
3:查询可见Drawable,并放入RenderQueue中进行排序
4:处理阴影
5:执行RenderPath渲染
*/
class CU_API Renderer : public Object
{
	DECLARE_RTTI(Renderer, Object, "REND")
public:
	Renderer();
	~Renderer();

	void update();

	Texture* getGBuffer(size_t width, size_t height, size_t format, bool cubemap, bool filtered, bool srgb, unsigned persistent);

private:
	typedef Vector<View*> ViewList;
	typedef std::map<String, RenderPath*> RenderPathMap;
	typedef std::map<uint64, Texture*>		TextureMap;
	Device*			m_device;
	RenderPathMap	m_renderer;
	ViewList		m_views;
	TextureMap		m_gbuffers;
	RenderTarget*	m_target;		// ??
};

CU_NS_END
