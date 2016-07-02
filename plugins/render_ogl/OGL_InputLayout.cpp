#include "OGL_InputLayout.h"
#include "OGL_Mapping.h"

CU_NS_BEGIN

OGL_InputLayout::OGL_InputLayout(const InputElement* elements, size_t count)
	: InputLayout(elements, count)
{
}

OGL_InputLayout::~OGL_InputLayout()
{
}

void OGL_InputLayout::bind()
{
	// ≤ª π”√vao
}

CU_NS_END