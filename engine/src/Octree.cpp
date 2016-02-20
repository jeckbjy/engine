#include "Octree.h"

CU_NS_BEGIN

Octant::Octant(const AABox& box, Octant* parent, Octree* root, size_t level, size_t index /* = ROOT_INDEX */)
: m_root(root)
, m_parent(parent)
, m_level(level)
, m_index(index)
, m_count(0)
{
	for (int i = 0; i < OCTANT_NUMS; ++i)
		m_children[i] = 0;
	setBox(box);
}

Octant::~Octant()
{
	clear();
}

void Octant::clear()
{
	for (int i = 0; i < OCTANT_NUMS; ++i)
	{
		if (m_children[i])
		{
			delete m_children[i];
			m_children[i] = 0;
		}
	}
}

Octant* Octant::getChild(size_t index)
{
	return NULL;
}

void Octant::setBox(const AABox& box)
{
	m_center = box.getCenter();
	m_halfSize = box.getHalfSize();

	m_worldBox = box;
	m_cullingBox = AABox(box.getMin() - m_halfSize, box.getMax() + m_halfSize);
}

bool Octant::checkBoxFit(const AABox& box) const
{
	Vector3 boxSize = box.getSize();
	// If max split level, size always OK, otherwise check that box is at least half size of octant
	if (m_level >= m_root->getMaxLevel() ||
		boxSize.x >= m_halfSize.x ||
		boxSize.y >= m_halfSize.y ||
		boxSize.z >= m_halfSize.z)
	{
		return true;
	}
	// Also check if the box can not fit a child octant's culling box, in that case size OK (must insert here)
	if (box.getMin().x <= m_worldBox.getMin().x - 0.5f * m_halfSize.x ||
		box.getMax().x <= m_worldBox.getMax().x + 0.5f * m_halfSize.x ||
		box.getMin().y <= m_worldBox.getMin().y - 0.5f * m_halfSize.y ||
		box.getMax().y <= m_worldBox.getMax().y + 0.5f * m_halfSize.y ||
		box.getMin().z <= m_worldBox.getMin().z - 0.5f * m_halfSize.z ||
		box.getMax().z <= m_worldBox.getMax().z + 0.5f * m_halfSize.z )
	{
		return true;
	}

	// Bounding box too small, should create a child octant
	return false;
}

void Octant::insert(Drawable* drawable)
{
	const AABox& box = drawable->getWorldBox();
	// If root octant, insert all non-occludees here, so that octant occlusion does not hide the drawable.
	// Also if drawable is outside the root octant bounds, insert to root
	bool insertHere;
	if (this == m_root)
		insertHere = drawable->isOccludee() || m_cullingBox.contains(box) || checkBoxFit(box);
	else
		insertHere = checkBoxFit(box);

	if (insertHere)
	{
		// old
	}
	else
	{// 需要放到子节点中
		Vector3 center = box.getCenter();
		size_t x = center.x < m_center.x ? 0 : 1;
		size_t y = center.y < m_center.y ? 0 : 2;
		size_t z = center.z < m_center.z ? 0 : 4;

		getChild(x + y + z)->insert(drawable);
	}
}

void Octant::add(Drawable* drawable)
{

}

void Octant::remove(Drawable* drawable)
{

}

CU_NS_END
