#pragma once
#include "Asset.h"
#include "Graphics.h"
#include "Skeleton.h"
#include "Animation.h"
#include "Material.h"

CU_NS_BEGIN

/*
�������������棺��Unity,GamePlay
MeshPart�г�����һ��Material���ʣ���ΪĬ�ϣ����ԴӴ����м���
*/
struct CU_API MeshPart : public Ref
{
	VertexLayoutPtr	vertices;	// ������Ϣ�����ṹ��Ϣ
	IndexBufferPtr	indexs;		// ������
	uint8_t			topology;	// Topology�ṹ
	uint32_t		offset;		// ��ʼƫ�ƣ�ͨ��Ϊ0
	uint32_t		count;		// ��������
	MaterialPtr		material;	// ����,�ŵ���ߣ���
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
ģ��������Ϣ����Ҫ�������������ݣ�Mesh(��Material),Skeleton,Animation
���п��Զ�����������ֻ��������һ������Ϣ��Ҳ�������ӵ�����Model��
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
	uint8_t		m_flags;		// ��ʶ��Ҫ�����Ĳ�������
	Mesh*		m_mesh;
	Skeleton*	m_skeleton;
	Animation*	m_animation;
};

CU_NS_END
