#include "OGL_VertexArray.h"
#include "OGL_Mapping.h"
#include "OGL_Program.h"
#include "OGL_Buffer.h"

CU_NS_BEGIN

OGL_VertexArray::OGL_VertexArray(VertexLayout* layout)
	: VertexArray(layout)
{

}

OGL_VertexArray::~OGL_VertexArray()
{
	destroy();
}

void OGL_VertexArray::bind(OGL_Program* prog)
{
#ifdef CU_USE_VAO
	if (m_dirty)
	{
		m_dirty = false;
		destroy();
	}
	GLuint vao = 0;

	uint32_t progID = 0;
	VAOMap::iterator itor = m_vaos.find(progID);
	if (itor != m_vaos.end())
	{
		vao = itor->second;
		glBindVertexArray(vao);
	}
	else
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		bindAttributes(prog);
		m_vaos[progID] = vao;
	}
#else
	bindAttributes(prog);
#endif
}

void OGL_VertexArray::unbind(OGL_Program* prog)
{
#ifdef CU_USE_VAO
	glBindVertexArray(0);
#else
	unbindAttributes(prog);
#endif
}

void OGL_VertexArray::bindAttributes(OGL_Program* prog)
{
	OGL_Buffer* last = NULL;
	OGL_Buffer* buff;
	GLint attr;
	GLint glByte;
	GLenum glType;
	GLboolean glNorm;
	// ²»Ê¹ÓÃvao
	const VertexLayout::ElementVec& elements = m_layout->getElements();
	for (size_t i = 0; i < elements.size(); ++i)
	{
		const VertexElement& elem = elements[i];
		buff = (OGL_Buffer*)m_buffers[elem.slot].get();
		attr = prog->getLocation(elem.semantic);
		if (buff == NULL || attr == -1)
			continue;

		if (last != buff)
		{
			last = buff;
			glBindBuffer(GL_ARRAY_BUFFER, buff->handle());
		}

		glByte = PixelUtil::getComponents(elem.format);
		glNorm = PixelUtil::isNormalized(elem.format);
		glType = OGL_Mapping::getGLType(elem.format);
		glVertexAttribPointer(attr, glByte, glType, glNorm, elem.stride, (void*)elem.offset);
		glEnableVertexAttribArray(attr);
	}
}

void OGL_VertexArray::unbindAttributes(OGL_Program* prog)
{
	const VertexLayout::ElementVec& elements = m_layout->getElements();
	for (size_t i = 0; i < elements.size(); ++i)
	{
		const VertexElement& elem = elements[i];
		GLint attr = prog->getLocation(elem.semantic);
		if (attr != -1)
			glDisableVertexAttribArray(attr);
	}
}

void OGL_VertexArray::destroy()
{
#ifdef CU_USE_VAO
	GLuint vao;
	for (VAOMap::iterator itor = m_vaos.begin(); itor != m_vaos.end(); ++itor)
	{
		vao = itor->second;
		glDeleteVertexArrays(1, &vao);
	}
	m_vaos.clear();
#endif
}

CU_NS_END