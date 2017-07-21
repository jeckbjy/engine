#pragma once
#include "RXP_API.h"

CUTE_NS_BEGIN

// ForwardRenderer
// DefferRenderer, ForwardPlusRenderer
class CUTE_CORE_API RXP_Renderer : public Renderer
{
public:
    RXP_Renderer();
    ~RXP_Renderer();
    
    void init();
    void term();
    
    void update();
    void render();
    
private:
    
};

CUTE_NS_END
