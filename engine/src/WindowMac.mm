//! Application
#include "Cute/Platform.h"

#if (CUTE_OS_MAC)
#include "Cute/Window.h"

#import <Cocoa/Cocoa.h>

CUTE_NS_BEGIN

class CUTE_CORE_API WindowMac : public Window
{
public:
    WindowMac(int styles, void* parent)
        : Window(styles)
    {
//        m_view = [[NSView alloc] init];
        NSRect rc = NSMakeRect(0, 0, 800, 600);
        NSUInteger uistyles = NSWindowStyleMaskTitled | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable;
        m_window = [[NSWindow alloc] initWithContentRect:rc styleMask:uistyles backing:NSBackingStoreBuffered defer:NO];

        [m_window setTitle:@"HelloWin Test"];
        [m_window makeKeyAndOrderFront:m_window];
//        [m_window makeMainWindow];
        [m_window center];
        //        [m_window retain];
        
//        [m_window setContentView:m_view];
//        [m_window setAutodisplay:YES];
//        [m_window setAcceptsMouseMovedEvents:YES];
//        [m_window setReleasedWhenClosed:YES];
//        [m_window setExcludedFromWindowsMenu:YES];
//        [m_window setIgnoresMouseEvents:YES];
//        
//#if defined (MAC_OS_X_VERSION_10_7) && (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7)
//        if ((styles & (STYLE_HAS_MIN_BUTTON | STYLE_HAS_TITLE_BAR)) == (STYLE_HAS_MIN_BUTTON | STYLE_HAS_TITLE_BAR))
//            [m_window setCollectionBehavior: NSWindowCollectionBehaviorFullScreenPrimary];
//        
//        if ([m_window respondsToSelector: @selector (setRestorable:)])
//            [m_window setRestorable: NO];
//#endif
    }
    
    ~WindowMac()
    {
//        [m_view release];
        
        [m_window close];
        [m_window release];
    }
    
    void* getNative() const
    {
        return m_window;
    }
    
    void setVisible(bool flag)
    {
        if(flag)
            [m_window orderFront:nil];
//            [m_window makeKeyAndOrderFront:nil];
        else
            [m_window orderOut:nil];
    }
    
    void setTitle(const String& title)
    {
        [m_window setTitle:[NSString stringWithUTF8String:title.data()] ];
    }
    
    void setBounds(const Rect2I& bounds)
    {
//        NSRect rect;
//        NSSize oldViewSize;
//        [m_window setFrame:]
    }
    
    void getBounds(Rect2I& bounds) const
    {
        
    }
    
    void setMinimised(bool flag)
    {
        if(flag)
            [m_window miniaturize:nil];
        else
            [m_window deminiaturize:nil];
    }
    
    bool isMinimised() const
    {
        return [m_window isMiniaturized];
    }
    
    void setFullScreen(bool flag)
    {
        
    }
    
    bool isFullScreen() const
    {
        return false;
    }
    
    bool isKioskMode() const
    {
        
        return false;
    }
    
    bool setAlwaysOnTop(bool flag)
    {
//        int style = hasFlag(STYLE_TEMPORARY) ? NSPopUpMenuWindowLevel : NSFloatingWindowLevel;
//        [m_window setLevel:flag ? style : NSNormalWindowLevel];
        return true;
    }
    
    void toFront(bool active)
    {
        
    }
    
    void toBehind(Window* other)
    {
        
    }
    
    bool isFocused() const
    {
        return false;
    }
    
    void gotFocus()
    {
        
    }
    
    void setAlpha(float alpha)
    {
        
    }
    
    void repaint(const Rect2I& area)
    {
        
    }
    
    bool contains(const Point& localPos, bool inChildWindow) const
    {
        return false;
    }
    
    Point toGlobal(const Point& localPos)
    {
        return Point();
    }
    
    Point toLocal(const Point& globalPos)
    {
        return Point();
    }
    
protected:
    NSWindow*   m_window = nil;
    NSView*     m_view = nil;
};

Window* Window::create(int styles, void *parent)
{
    return new WindowMac(styles, parent);
}

CUTE_NS_END

#endif
