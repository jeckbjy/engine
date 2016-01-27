#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

class OGLProgram;
class CU_OGL_API OGLPipeline : public Pipeline
{
public:
	OGLPipeline(const PIPELINE_DESC& desc);
	~OGLPipeline();

};

CU_NS_END