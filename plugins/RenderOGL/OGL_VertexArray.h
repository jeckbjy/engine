#pragma once
#include "OGL_API.h"
#include <vector>

CUTE_NS_BEGIN

// vao
class OGL_Program;
class CU_OGL_API OGL_VertexArray : public VertexArray
{
public:
	OGL_VertexArray(uint32_t id, VertexLayout* layout);
	~OGL_VertexArray();

	void bind(OGL_Program* prog);
	void unbind(OGL_Program* prog);

private:
	void bindAttributes(OGL_Program* prog);
	void unbindAttributes(OGL_Program* prog);
	void destroy();

private:
	typedef std::map<uint32_t, GLuint> VAOMap;

#ifdef CU_USE_VAO
	VAOMap m_vaos;
#endif
};

CUTE_NS_END
