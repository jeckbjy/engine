#pragma once
#include "Cute/Application.h"
#include "Cute/Window.h"
#include "Cute/RenderAPI.h"
using namespace Cute;

class App3D : public Application
{
public:
    static void launch();
    
public:
    App3D();
    ~App3D();
    
    void run();
    
    virtual bool init() { return true; }
    virtual void quit() {}
    virtual void draw() {}
    
protected:
    IDevice*        m_device;
    ICommandQueue*  m_cmdQueue;
    ICommandList*   m_cmdBuffer;
    Window*         m_window;
    ISwapChain*     m_target;
    
};

class TriangleApp : public App3D
{
public:
    bool init();
//    void quit();
//    void draw();
    
protected:
    IBuffer*        m_vb;
    IBuffer*        m_ib;
};

class TextureApp : public App3D
{
public:
    bool init();
    
protected:
    ITexture*    m_texture;
};

class MeshApp : public App3D
{
public:
    bool init();
protected:
    
};

class AnimationApp : public App3D
{
public:
    bool init();
    
protected:
    
};
