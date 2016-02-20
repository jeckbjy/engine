#pragma once
#include "API.h"
#include "AABox.h"
#include "Drawable.h"

CU_NS_BEGIN

class Octree;
class Octant
{
public:
	enum
	{
		OCTANT_NUMS = 8,
		ROOT_INDEX = 0xffffffff,
	};

	Octant(const AABox& box, Octant* parent, Octree* root, size_t level, size_t index = ROOT_INDEX);
	~Octant();
	
	void clear();
	void setBox(const AABox& box);

	Octant* getChild(size_t index);

	bool checkBoxFit(const AABox& box) const;

	void insert(Drawable* drawable);
	void add(Drawable* drawable);
	void remove(Drawable* drawable);

private:
	typedef Vector<Drawable*> DrawableVec;
	Octree*		m_root;
	Octant*		m_parent;
	Octant*		m_children[OCTANT_NUMS];
	size_t		m_level;
	size_t		m_index;		// relative to its sibling or root_index
	AABox		m_worldBox;
	AABox		m_cullingBox;
	Vector3		m_center;
	Vector3		m_halfSize;
	DrawableVec m_drawables;
	size_t		m_count;		// drawable总数目,包括子节点中的
};

// 八叉树管理可渲染物体
class Octree : public Octant
{
public:
	Octree();
	~Octree();

	size_t getMaxLevel() const { return m_levels; }

protected:
	Octant	m_root;		// 根节点
	size_t	m_levels;	// 共有多少层
};

CU_NS_END
