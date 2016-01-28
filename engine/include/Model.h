#pragma once
#include "Asset.h"
#include "Graphics.h"
#include "Skeleton.h"
#include "Animation.h"
#include "Material.h"

CU_NS_BEGIN

// 需要包含的信息：Material：
class CU_API Renderable
{

};

// 一个最基本的渲染提
struct RenderData : public Ref
{
	Material*		material;
	VertexLayout*	vertex;
	IndexBuffer*	index;
	uint8_t			topology;
	size_t			veritces_num;
	size_t			vertices_off;
	size_t			instance_num;
	size_t			instance_off;
	RenderData() : vertex(NULL), index(NULL), topology(PT_TRIANGLE_LIST),
		veritces_num(0), vertices_off(0), instance_num(0), instance_off(0)
	{}
};

class CU_API SubMesh
{

};

class CU_API Mesh
{

};

// 模型数据,需要包含：Mesh, Materail, Skeleton,Animation
// 所有资源的加载都需要通过Model,即使分开的：
class CU_API Model : public Asset
{
public:
	Model();
	~Model();

	bool load(Stream* stream);
	void save(Stream* stream);

	Mesh* getMesh() const { return m_mesh; }
	Skeleton* getSkeleton() const { return m_skeleton; }
	Animation* getAnimation() const { return m_animation; }

private:
	Mesh*		m_mesh;
	Skeleton*	m_skeleton;
	Animation*	m_animation;
};

CU_NS_END
