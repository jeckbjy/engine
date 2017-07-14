#pragma core
#include "Cute/Foundation.h"
#include "Cute/Rect2.h"
#include "Cute/Vector2.h"

CUTE_NS_BEGIN

class Window
{
public:
    static Window* create(int styles, void* parent);
    
    typedef Vector2 Point;
    
    enum StyleFlags
    {
        STYLE_APPEARS_ON_TASKBAR    = (1 << 0),     // ignored on MacOSX
        STYLE_TEMPORARY             = (1 << 1),     // popup windows like menu, tooltip,etc
        STYLE_HAS_TITLE_BAR         = (1 << 2),
        STYLE_HAS_MIN_BUTTON        = (1 << 3),
        STYLE_HAS_Max_BUTTON        = (1 << 4),
        STYLE_HAS_CLOSE_BUTTON      = (1 << 5),
        STYLE_HAS_DROP_SHADOW       = (1 << 6),
        STYLE_RESIZABLE             = (1 << 7),
        STYLE_REPAINTED_EXPLICTLY   = (1 << 8),
        STYLE_IGNORE_MOUSE_CLICK    = (1 << 9),
        STYLE_IGNORE_KEY_PRESS      = (1 << 10),
        STYLE_IS_SEMI_TRANSPARENT   = (1 << 11),
        STYLE_IS_FULLSCREEN         = (1 << 12),
        STYLE_DEFAULT               = STYLE_RESIZABLE,
    };
    
public:
    Window(int styles);
    virtual ~Window();
    
    uint32 getStyles() const noexcept;
    uint32 getUniqueID() const noexcept;
    
    virtual void* getNative() const = 0;
    virtual void setVisible(bool flag) = 0;
    virtual void setTitle(const String& title) = 0;
    virtual void setBounds(const Rect2I& bounds) = 0;
    virtual void getBounds(Rect2I& bounds) const = 0;

    virtual void setMinimised(bool flag) = 0;
    virtual bool isMinimised() const = 0;
    
    virtual void setFullScreen(bool flag)= 0;
    virtual bool isFullScreen() const = 0;
    
    virtual bool isKioskMode() const;
    
    virtual bool setAlwaysOnTop(bool flag) = 0;
    virtual void toFront(bool active) = 0;
    virtual void toBehind(Window* other) = 0;
    
    virtual bool isFocused() const = 0;
    virtual void gotFocus() = 0;
    
    virtual void setAlpha(float alpha) = 0;
    virtual void repaint(const Rect2I& area) = 0;
    
//    virtual void setIcon() = 0;
    
    virtual bool contains(const Point& localPos, bool inChildWindow) const = 0;
    
    virtual Point toGlobal(const Point& relativePosition) = 0;
    virtual Point toLocal(const Point& screenPosition) = 0;
    
protected:
    uint32 m_styles;
    uint32 m_uniqueID;
};

CUTE_NS_END
