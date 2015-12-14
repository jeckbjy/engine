#pragma once
#include "Drawable.h"
#include "UIView.h"

CU_NS_BEGIN

// 层级？？
class CU_API Layer
{

};

// 用于绘制所有ui控件，可以有三种空间
class CU_API Canvas : public Drawable
{
public:
	Canvas();
	~Canvas();

private:
	UIView* m_view;
};

CU_NS_END