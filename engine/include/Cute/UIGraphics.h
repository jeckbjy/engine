#pragma once
#include "Cute/Foundation.h"
#include "Cute/Rect2.h"

CUTE_NS_BEGIN


class UIPath
{
    
};

class Image;
class UIGraphics
{
public:
    UIGraphics();
    ~UIGraphics();
    
    void setColor();
    
    void drawText(const String& text);
    void drawRect(const Rect2&  rect, int thickness = 1);
    void drawLine(const Vector2& spos, const Vector2& epos);
    void drawEllipse(const Rect2& rect, float thickness);
    void drawImage(const Image& image, const Rect2& src, const Rect2& dst);
    //    void drawArrow();
    
    void fillRect(const Rect2& area);
    void fillPath();
};

// 基于mesh的绘制
class UIBatch : public UIGraphics
{
public:
private:
};

CUTE_NS_END
