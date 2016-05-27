#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

class OGL_Shader;
class OGL_Program;
class CU_OGL_API OGL_Pipeline : public Pipeline
{
public:
	OGL_Pipeline(const PipelineDesc& desc);
	~OGL_Pipeline();

	void bind();

private:
	RenderStateDesc*	m_states;
	OGL_Program*	m_program;
};

CU_NS_END