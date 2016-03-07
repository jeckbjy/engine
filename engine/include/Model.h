#pragma once
#include "Asset.h"
#include "Graphics.h"
#include "Geometry.h"
#include "Material.h"
#include "Skeleton.h"
#include "Animation.h"

CU_NS_BEGIN

// lod support
class CU_API Mesh : public Ref
{
public:
	Mesh(){}
	~Mesh(){}

	bool load(Stream* stream);
	void save(Stream* stream);

	void setGeometry(size_t index, size_t lod, Geometry* geo);
	void setMaterial(size_t index, Material* mat);
	void addMaterial(Material* mat);

	size_t getMaterialSize() const { return m_materials.size(); }
	size_t getGeometrySize() const { return m_geometries.size(); }
	size_t getGeometryLodLevel(size_t index) const { return m_geometries[index].size(); }

	Geometry* getGeometry(size_t index, float distance);
	Geometry* getGeometry(size_t index, size_t lod) { return m_geometries[index][lod]; }
	Material* getMaterial(size_t index) { return m_materials[index]; }

protected:
	friend class Model;
	typedef Vector<Geometry*>	GeometryLod;
	typedef Vector<GeometryLod>	GeometryVec;
	typedef Vector<Material*>	MaterialVec;
	GeometryVec m_geometries;
	MaterialVec m_materials;
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
