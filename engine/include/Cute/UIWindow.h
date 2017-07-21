#pragma once
#include "Cute/UIElement.h"

CUTE_NS_BEGIN

class UIWindow : public UIElement
{
public:
    UIWindow();
    ~UIWindow();
    
    void paint(UIGraphics& g);
};

CUTE_NS_END
