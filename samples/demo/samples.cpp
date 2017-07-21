#include "samples.h"
#include "Cute/PluginManager.h"

void App3D::launch()
{
    TriangleApp app;
    app.run();
}

App3D::App3D()
{
    
}

App3D::~App3D()
{
    
}

void App3D::run()
{
    // load plugin
//    Plugin
    PluginManager::instance().load("./libplugin_ogl.dll");
    
    m_device = RenderModule::instance().getDevice();
    m_window = Window::create(0, 0);
//    m_cmdQueue = m_device.
//    m_device->new
    
    if(!init())
        return;
    
    Application::run();
    quit();
}

bool TriangleApp::init()
{
    struct CustomVertex
    {
        Vector3 position;
        Vector3 color;
    };
    
//    InputElement elements[] = {
//        {},
//        {}
//    };
    
    float vertex_data[] = {
        -1.0f, -1.0f,  0.5773f,
        0.0f,  -1.0f, -1.15475f,
        1.0f,  -1.0f,  0.5773f,
        0.0f,   1.0f,  0.0f
    };
    
    short index_data[] = { 0, 1, 2 };
    
//    if(m_device->newBuffer(&BufferDesc(), &m_vb)
//       return false;
//       
//    if(m_device->newBuffer(&BufferDesc(), &m_ib))
//       return false;
    
//    if(m_device->newLayout())

       return true;
}

bool TextureApp::init()
{
    return true;
}

