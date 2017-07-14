//! Application
#include "Cute/Platform.h"

#if defined(CUTE_USE_X11)
#include "Cute/Window.h"

CUTE_NS_BEGIN

// todo:xcb
class WindowX11 : public Window
{
public:
};

CUTE_NS_END

#endif

