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

}

void OGL_VertexArray::bind(OGL_Program* prog)
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
	}
}

CU_NS_END