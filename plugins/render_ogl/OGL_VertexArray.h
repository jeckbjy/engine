#pragma once
#include "OGL_API.h"
#include <vector>

CU_NS_BEGIN

// vao
class OGL_Program;
class CU_OGL_API OGL_VertexArray : public VertexArray
{
public:
	OGL_VertexArray(VertexLayout* layout);
	~OGL_VertexArray();

	void bind(OGL_Program* prog);
private:
	// 查找
};

// vao:需要三方面东西：layout,vbo,prog
//class CU_OGL_API OGL_InputLayout : public InputLayout
//{
//public:
//	OGL_InputLayout(const InputElement* elements, size_t count);
//	~OGL_InputLayout();
//
//	void bind(OGL_Program* prog, OGL_Buffer* buffers);
//};

CU_NS_END