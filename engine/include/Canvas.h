#pragma once
#include "Drawable.h"
#include "UIView.h"

CU_NS_BEGIN

// �㼶����
class CU_API Layer
{

};

// ���ڻ�������ui�ؼ������������ֿռ�
class CU_API Canvas : public Drawable
{
public:
	Canvas();
	~Canvas();

private:
	UIView* m_view;
};

CU_NS_END