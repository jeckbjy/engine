#include "OGL_Pipeline.h"
#include "OGL_Program.h"

CUTE_NS_BEGIN


OGL_Pipeline::OGL_Pipeline()
{
//	m_program = (OGL_Program*)desc.program;
//	if (desc.states)
//	{
//		m_states = new RenderStateDesc();
//		*m_states = *desc.states;
//	}
}

OGL_Pipeline::~OGL_Pipeline()
{
    term();
}

bool OGL_Pipeline::init(const GraphicsPipelineDesc &desc)
{
    return false;
}

bool OGL_Pipeline::init(const ComputePipelineDesc &desc)
{
    return false;
}

void OGL_Pipeline::term()
{
    
}

void OGL_Pipeline::bind()
{
//	m_program->bind();
//	if (!m_states)
//		return;

}

CUTE_NS_END
