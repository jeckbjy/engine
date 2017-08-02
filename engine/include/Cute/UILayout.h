#pragma once
#include "Cute/UIElement.h"

CUTE_NS_BEGIN

class CUTE_CORE_API UILayout
{
public:
    virtual void onLayout(){}
};

class CUTE_CORE_API UILinearLayout : public UILayout
{
public:
    /** 水平布局 */
    static const uint8 ORITENTATION_HORIZONTAL  = 0x0;
    /** 垂直布局 */
    static const uint8 ORITENTATION_VERTICAL    = 0x1;
    
    void onMeasure();
    
protected:
    uint8 mOrientation;
};

class CUTE_CORE_API UIRelativeLayout : public UILayout
{
    
};

class CUTE_CORE_API UIGridLayout : public UILayout
{
    
};

CUTE_NS_END
