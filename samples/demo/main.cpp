#include "Cute/Application.h"
#include "Cute/Window.h"
using namespace Cute;
#include "test.h"

int main(int argc, char* argv[])
{
//    return runApp();
//    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
//    NSApplication* app = [NSApplication sharedApplication];
    
    Window* win = Window::create(Window::STYLE_DEFAULT, NULL);
    Application app;
    app.run(argc, argv);
    delete win;
//    return 0;
}
