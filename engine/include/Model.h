#pragma once
#include "Asset.h"
#include "Graphics.h"
#include "Skeleton.h"
#include "Animation.h"
#include "Material.h"

CU_NS_BEGIN

/*
区别于其他引擎：如Unity,GamePlay
MeshPart中持有了一个Material材质，作为默认，可以从磁盘中加载
*/
struct CU_API MeshPart : public Ref
{
	VertexLayoutPtr	vertices;	// 顶点信息，含结构信息
	IndexBufferPtr	indexs;		// 面索引
	uint8_t			topology;	// Topology结构
	uint32_t		offset;		// 起始偏移，通常为0
	uint32_t		count;		// 顶点数量
	MaterialPtr		material;	// 材质,放到外边？？
};
typedef SharedPtr<MeshPart> MeshPartPtr;

class CU_API Mesh : public Ref
{
public:
	Mesh(){}
	~Mesh(){}

	bool load(Stream* stream);
	void save(Stream* stream);

	void add(MeshPart* part) { m_parts.push_back(part); }
	size_t size() const { return m_parts.size(); }
	MeshPart* at(size_t index) { return m_parts[index]; }

protected:
	friend class Model;
	typedef std::vector<MeshPart*> Parts;
	Parts m_parts;
};

/*
模型数据信息：需要包含三部分数据：Mesh(含Material),Skeleton,Animation
其中可以都包含，或者只包含其中一部分信息，也可以链接到其他Model中
*/
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
	uint8_t		m_flags;		// 标识需要保存哪部分数据
	Mesh*		m_mesh;
	Skeleton*	m_skeleton;
	Animation*	m_animation;
};

CU_NS_END
