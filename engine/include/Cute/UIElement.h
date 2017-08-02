#pragma once
#include "Cute/Foundation.h"
#include "Cute/UIGraphics.h"

CUTE_NS_BEGIN

// UICanvas
// UIWindow UIView UIButton UISwtich UITextField UITextView UISlider UIScrollView UIDatePicker
// UITableView
class CUTE_CORE_API UIElement
{
public:
    UIElement();
    ~UIElement();
    
    virtual void paint(UIGraphics& g) = 0;
    virtual bool hitTest(int x, int y) const;
    
    void repaint();
    void repaint(const Rect2& rect);
    
//    bool contains(const Point& pos);
//    
//    bool isVisible() const;
//    
//    void setPosition(int x, int y);
//    void setSize(int width, int height);
//    void setBounds(int x, int y, int width, int height);
//    
//    void setTransform();
//    
//    void setOpaque(bool opaque);
//    bool isOpaque() const;
//    
//    void setAlpha(float alpha);
//    float getAlpha() const;
//    
//    void setEnabled(bool enabled);
//    bool isEnabled() const;
    
protected:
    virtual void onMeasure(){}
    virtual void onLayout(){}
    
private:
    bool m_visible : 1;
    bool m_opaque  : 1;
    bool m_enabled : 1;
    bool m_focused : 1;
    
    float m_alpha;
};

CUTE_NS_END
