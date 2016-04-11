#pragma once
#include "Object.h"
#include "Graphics.h"

CU_NS_BEGIN

// �߲���Ⱦ,ȫ��Ψһ,TBR(tile-based render) or IMR()
/*
���Զ��߳�ִ��,ÿ���߳�ִ��һ��Camera��������Ⱦ��Ҫ��Camera˳��ִ��
��Ⱦ����:
1:����Camera
2:��������Camera����ÿ�����Camera����ִ��������Ⱦ
3:��ѯ�ɼ�Drawable,������RenderQueue�н�������
4:������Ӱ
5:ִ��RenderPath��Ⱦ
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
