#pragma once
#include "Object.h"
#include "Graphics.h"
#include "RenderPath.h"

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
class Camera;
class CU_API Renderer : public Object
{
	DECLARE_RTTI(Renderer, Object, "REND")
public:
	Renderer();
	~Renderer();

	void update();
	void render(Camera* camera);

private:
	typedef std::map<String, RenderPath*> RenderPathMap;
	RenderPathMap m_renderer;
};

CU_NS_END
