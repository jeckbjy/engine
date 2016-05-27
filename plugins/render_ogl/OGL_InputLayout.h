#pragma once
#include "OGL_API.h"
#include "OGL_Program.h"
#include <vector>

CU_NS_BEGIN

// vao:需要三方面东西：layout,vbo,prog
class CU_OGL_API OGL_InputLayout : public InputLayout
{
public:
	OGL_InputLayout(const InputElement* elements, size_t count);
	~OGL_InputLayout();
	//void release();

//	void bind(OGLProgram* prog);
//	void unbind(OGLProgram* prog);
//
//	void bind_attrs(OGLProgram* prog);
//private:
//#ifdef CU_USE_VAO
//	typedef std::map<OGLProgram*, GLuint> VAOMap;
//	VAOMap m_vaos;
//#endif
//	VertexDeclaration m_desc;
};

CU_NS_END