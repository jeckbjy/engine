#include "OGL_InputLayout.h"
#include "OGL_Mapping.h"

CU_NS_BEGIN

OGLInputLayout::OGLInputLayout(const InputElement* elements, size_t count)
	: InputLayout(elements, count)
{
}

OGLInputLayout::~OGLInputLayout()
{
	//release();
}

//void OGLInputLayout::release()
//{
//#ifdef CU_USE_VAO
//	for (VAOMap::iterator itor = m_vaos.begin(); itor != m_vaos.end(); ++itor)
//	{
//		glDeleteVertexArrays(1, &(itor->second));
//	}
//	m_vaos.clear();
//#endif
//}
//
//void OGLInputLayout::bind(OGLProgram* prog)
//{
//	if (m_dirty)
//	{
//		release();
//		m_dirty = false;
//	}
//#ifdef CU_USE_VAO
//	// ���Ҳ����ã��������򴴽�
//	VAOMap::iterator itor = m_vaos.find(prog);
//	if (itor != m_vaos.end())
//	{
//		glBindVertexArray(itor->second);
//		return;
//	}
//	// û���ҵ����½�
//	GLuint vao;
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//	bind_attrs(prog);
//	m_vaos[prog] = vao;
//#else
//	bind_attrs(prog);
//#endif
//}
//
//void OGLInputLayout::unbind(OGLProgram* prog)
//{
//#ifdef CU_USE_VAO
//	glBindVertexArray(0);
//#else
//	for (size_t i = 0; i < m_desc.size(); ++i)
//	{
//		const VertexElement& el = m_desc[i];
//		GLint attr = prog->getAttribute(el.usage);
//		if (attr != -1)
//		{
//			glDisableVertexAttribArray(attr);
//		}
//	}
//#endif
//}
//
//void OGLInputLayout::bind_attrs(OGLProgram* prog)
//{
//	// �Ȱ�vertex buffer
//	for (size_t i = 0; i < m_buffers.size(); ++i)
//	{
//		((OGLBuffer*)m_buffers[i])->bind();
//	}
//
//	for (size_t i = 0; i < m_desc.size(); ++i)
//	{
//		const VertexElement& el = m_desc[i];
//		GLint attr = prog->getAttribute(el.usage);
//		if (attr == -1)
//			continue;
//		GLenum type = OGLMapping::getGLType(el.type);
//		GLboolean normalized = PixelUtil::isNormalized(el.type);
//		glVertexAttribPointer(attr, (GLint)PixelUtil::getElementCount(el.type), type, normalized, el.stride, (void*)el.offset);
//		glEnableVertexAttribArray(attr);
//	}
//}

CU_NS_END