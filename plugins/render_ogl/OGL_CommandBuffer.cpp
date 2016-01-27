#include "OGL_CommandBuffer.h"
#include "OGL_Mapping.h"
#include "OGL_Buffer.h"
#include "OGL_Program.h"
#include "OGL_VertexLayout.h"
#include "OGL_Pipeline.h"

CU_NS_BEGIN

OGLCommandBuffer::OGLCommandBuffer()
: _target(NULL)
, _pipeline(NULL)
, _layout(NULL)
, _ib(NULL)
, _descriptors(NULL)
, _primitive(PT_TRIANGLE_LIST)
, _verticesNum(0)
, _verticesOff(0)
, _instanceNum(0)
, _instanceOff(0)
, _vertexBase(0)
{
}

OGLCommandBuffer::~OGLCommandBuffer()
{

}

void OGLCommandBuffer::setViewport(int x, int y, size_t w, size_t h)
{
	glViewport(x, y, w, h);
}

void OGLCommandBuffer::setBlendFactor(const float factors[4])
{
	memcpy(_factors, factors, sizeof(_factors));
}

void OGLCommandBuffer::setStencilRef(size_t stencil)
{
	_stencil_ref = stencil;
}

void OGLCommandBuffer::setRenderTarget(RenderTarget* target)
{
	_target = target;
}

void OGLCommandBuffer::setDescriptorSet(DescriptorSet* descriptors)
{
	_descriptors = descriptors;
}

void OGLCommandBuffer::setTopology(Topology primitive)
{
	_primitive = primitive;
}

void OGLCommandBuffer::setPipeline(Pipeline* pipeline)
{
	_pipeline = (OGLPipeline*)pipeline;
}

void OGLCommandBuffer::setVertexLayout(VertexLayout* layout)
{
	_layout = (OGLVertexLayout*)layout;
}

void OGLCommandBuffer::setIndexBuffer(IndexBuffer* ib)
{
	_ib = (OGLBuffer*)ib;
}

void OGLCommandBuffer::draw(size_t vnum, size_t voff, size_t instance_num, size_t instance_off)
{
	_verticesNum = vnum;
	_verticesOff = voff;
	_instanceNum = instance_num;
	_instanceOff = instance_off;
	_ib = NULL;
}

void OGLCommandBuffer::drawIndexed(size_t inum, size_t ioff, size_t instance_num, size_t instance_off, int vertex_base)
{
	_verticesNum = inum;
	_verticesOff = ioff;
	_instanceNum = instance_num;
	_instanceOff = instance_off;
	_vertexBase = vertex_base;
}

void OGLCommandBuffer::dispatch(size_t group_x, size_t group_y, size_t group_z)
{

}

void OGLCommandBuffer::execute()
{
	//Instance渲染  http://www.zwqxin.com/archives/opengl/talk-about-geometry-instancing.html
	//详细介绍:  http://book.2cto.com/201412/48549.html
	GLint mode = OGLMapping::getPrimitiveMode(_primitive);
	// 绑定shader
	OGLProgram* prog = (OGLProgram*)_pipeline->getProgram();
	prog->bind(_descriptors);
	_layout->bind(prog);
	if (_verticesNum == 0)
	{
		_verticesNum = _ib ? _ib->count() : _layout->vertices();
	}
	if (_instanceNum <= 1)
	{
		if (_ib != NULL)
		{
			_ib->bind();
			GLenum gl_type = _ib->isIndex16() ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
			// count指顶点个数
			glDrawElements(mode, _verticesNum, gl_type, 0);
		}
		else
		{
			// 注：参数count是Vertex顶点个数
			glDrawArrays(mode, _verticesOff, _verticesNum);
		}
	}

	//glDrawElementsBaseVertex();	// 可以带有偏移
	//glDrawRangeElements();
	//// 主要是在shader中通过gl_InstanceID控制渲染
	//glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count  GLsizei primcount);
	//glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void *indicies,  GLsizei primcount);
	//glDrawElementsInstancedBaseVertex();
	//// 由gpu填充，cpu不知道
	//// http://www.cnblogs.com/clayman/archive/2011/10/25/2224532.html
	//glDrawArraysIndirect();
	//glDrawElementsIndirect();
	//if (cmd->primitive >= PT_CTRL_PT_PATCH_LIST1)
	//{
	//	int n = cmd->primitive - PT_CTRL_PT_PATCH_LIST1;
	//	glPatchParameteri(GL_PATCH_VERTICES, n);
	//}
	//// 绑定
	//// 执行渲染
	//if (cmd->ib != NULL)
	//{
	//	//glDrawElements();
	//}
}

CU_NS_END