#pragma once
#include "OGL_API.h"

CUTE_NS_BEGIN

class OGL_Shader;
class OGL_Program;
class CUTE_OGL_API OGL_Pipeline : public IPipeline
{
public:
	OGL_Pipeline(const PipelineDesc& desc);
	~OGL_Pipeline();

	void bind();

	OGL_Program* getProgram() { return m_program; }

private:
//	RenderStateDesc*	m_states;
//	OGL_Program*		m_program;
};

CUTE_NS_END
