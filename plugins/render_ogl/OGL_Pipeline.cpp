#include "OGL_Pipeline.h"
#include "OGL_Program.h"

CU_NS_BEGIN


OGL_Pipeline::OGL_Pipeline(const PipelineDesc& desc)
	: m_states(NULL)
{
	m_program = (OGL_Program*)desc.program;
	if (desc.states)
	{
		m_states = new RenderStateDesc();
		*m_states = *desc.states;
	}
}

OGL_Pipeline::~OGL_Pipeline()
{
	if (m_states)
		delete m_states;
}

void OGL_Pipeline::bind()
{
	m_program->bind();
	if (!m_states)
		return;

}

CU_NS_END