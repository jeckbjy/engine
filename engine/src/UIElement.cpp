//! UI
#include "Cute/UIElement.h"

CUTE_NS_BEGIN

UIElement::UIElement()
{
}

UIElement::~UIElement()
{
}

bool UIElement::hitTest(int x, int y) const
{
    return true;
}

void UIElement::repaint()
{
    
}

void UIElement::repaint(const Rect2 &rect)
{
    
}

CUTE_NS_END
