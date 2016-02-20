#include "ModelRender.h"

CU_NS_BEGIN

MeshSkin::MeshSkin(Skeleton* skeleton)
: m_skeleton(skeleton)
, m_matrixs(0)
, m_timestamp(0)
{
}

MeshSkin::~MeshSkin()
{
	reset();
}

void MeshSkin::reset()
{
	if (m_matrixs != 0)
	{
		delete[] m_matrixs;
		m_matrixs = 0;
		m_timestamp = 0;
	}
}

void MeshSkin::update(Transform* root, uint32 now)
{
	// ��ֹ���ͬʱ����
	if (m_timestamp == now)
		return;
	m_timestamp = now;
	size_t bone_count = m_skeleton->getBoneCount();

	// ����
	if (m_matrixs == 0)
	{
		m_matrixs = new Matrix4[bone_count];
	}
	// ÿ�β�ѯ��
	for (size_t i = 0; i < bone_count; ++i)
	{
		const Bone* bone = m_skeleton->getBone(i);
		Transform* node = root->findChild(bone->name, true);
		if (!node)
			continue;
		m_matrixs[i] = node->getWorldMatrix() * bone->pose;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
ModelRender::ModelRender(Model* model)
: m_model(model)
, m_skin(0)
{

}

ModelRender::~ModelRender()
{

}

void ModelRender::draw()
{
	// �ύ
}

CU_NS_END
