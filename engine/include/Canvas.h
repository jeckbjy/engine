#pragma once
#include "Drawable.h"
#include "UIView.h"

CU_NS_BEGIN

// ���ڻ�������ui�ؼ������������ֿռ�
class CU_API Canvas : public Drawable
{
	DECLARE_RTTI(Canvas, Component, 1)
public:
	Canvas();
	~Canvas();

	void draw(View* view);

private:
	Camera*	m_camera;	// ��ָ����Ϊ����overlay
	UIView* m_view;
};

CU_NS_END
