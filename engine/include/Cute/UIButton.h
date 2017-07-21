#pragma once
#include "Cute/UIElement.h"

CUTE_NS_BEGIN

class UIButton : public UIElement
{
public:
    UIButton();
    ~UIButton();
    
    void paint(UIGraphics& g);
};

CUTE_NS_END
