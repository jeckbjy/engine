#pragma once
#include "Drawable.h"
#include "UIView.h"

CU_NS_BEGIN

// 用于绘制所有ui控件，可以有三种空间
class CU_API Canvas : public Drawable
{
	DECLARE_RTTI(Canvas, Component, 1)
public:
	Canvas();
	~Canvas();

	void draw(View* view);

private:
	Camera*	m_camera;	// 不指定则为顶层overlay
	UIView* m_view;
};

CU_NS_END
