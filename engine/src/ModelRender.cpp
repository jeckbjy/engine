#include "ModelRender.h"
#include "View.h"
#include "Camera.h"

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
	// 防止多个同时更新
	if (m_timestamp == now)
		return;
	m_timestamp = now;
	size_t bone_count = m_skeleton->getBoneCount();

	// 创建
	if (m_matrixs == 0)
	{
		m_matrixs = new Matrix4[bone_count];
	}
	// 每次查询？
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

void ModelRender::draw(View* view)
{
	if (!m_model)
		return;
	const Vector3 DOT_SCALE(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);
	Mesh* mesh = m_model->getMesh();
	const AABox& box = getWorldBox();
	// 根据摄像机查询Model
	Camera* camera = view->getCamera();
	float distance = camera->getDistance(box.getCenter());
	float scale = box.getSize().dot(DOT_SCALE);
	float lodDistance = camera->getLodDistance(distance, scale, 1.0f);
	// 计算Geometry
	if (!m_skin)
	{// 静态模型
		Material* mat;
		Geometry* geo;
		size_t geo_index;
		size_t geo_size = mesh->getGeometrySize();
		for (size_t i = 0; i < mesh->getMaterialSize(); ++i)
		{
			geo_index = i % geo_size;
			mat = mesh->getMaterial(i);
			geo = mesh->getGeometry(i, lodDistance);
			// 创建batch
			Batch* batch = new Batch();
			batch->material = mat;
			batch->geometry = geo;
			batch->distance = distance;
			view->addBatch(batch);
		}
	}
	else
	{// 蒙皮信息

	}
}

CU_NS_END
