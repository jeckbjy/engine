//! Application
#include "Cute/Foundation.h"

#if defined(CUTE_OS_WIN_NT)

#include "Cute/Window.h"

CUTE_NS_BEGIN

class WindowWin32 : public Window
{
public:
    WindowWin32(int styles, void* parent)
		: Window(styles)
    {
        
    }
    
    ~WindowWin32()
    {
        
    }
    
    void* getNative() const OVERRIDE
    {
        return m_handle;
    }
    
    void setVisible(bool flag) OVERRIDE
    {
        ::ShowWindow(m_handle, flag ? SW_SHOWNA : SW_HIDE);
        
        if(flag)
            InvalidateRect(m_handle, 0, 0);
        
    }
    
    void setTitle(const String& title) OVERRIDE
    {
        SetWindowTextA(m_handle, title.data());
    }
    
    void setBounds(const Rect2I& bounds) OVERRIDE
    {
        
    }
    
    void getBounds(Rect2I& bounds) const OVERRIDE
    {
        
    }
    
    void setMinimised(bool flag) OVERRIDE
    {
        if(flag != isMinimised())
            ShowWindow(m_handle, flag ? SW_MINIMIZE : SW_SHOWNORMAL);
    }
    
    bool isMinimised() const OVERRIDE
    {
        WINDOWPLACEMENT wp;
        wp.length = sizeof (WINDOWPLACEMENT);
        GetWindowPlacement (m_handle, &wp);
        
        return wp.showCmd == SW_SHOWMINIMIZED;
    }
    
    void setFullScreen(bool flag) OVERRIDE
    {
        setMinimised(false);
    }
    
    bool isFullScreen() const OVERRIDE
    {
		return false;
        //if (! hasTitleBar())
        //    return fullScreen;
        //
        //WINDOWPLACEMENT wp;
        //wp.length = sizeof (wp);
        //GetWindowPlacement (hwnd, &wp);
        //
        //return wp.showCmd == SW_SHOWMAXIMIZED;
    }
    
private:
    HWND m_handle;
};

Window* Window::create(int styles, void* parent)
{
	return NULL;
	//return new WindowWin32(styles, parent);
}

CUTE_NS_END
#endif
