#include "Model.h"
#include "AssetCache.h"

CU_NS_BEGIN

const uint32_t	MODEL_MAGIC = CU_FOURCC('Q','M', 'O', 'D');
const uint8_t	MODEL_VERSION = 0x01;
const uint8_t	ENDS_TAG = 0x00;
const uint8_t	MESH_TAG = 0x01;
const uint8_t	BONE_TAG = 0x02;
const uint8_t	ANIM_TAG = 0x04;

const uint8_t	MESH_LINK = 0x08;
const uint8_t	BONE_LINK = 0x10;
const uint8_t	ANIM_LINK = 0x20;

bool Mesh::load(Stream* stream)
{
	return true;
}

void Mesh::save(Stream* stream)
{
	size_t count;
	count = m_parts.size();
	stream->writeVariant(count);
	for (size_t i = 0; i < count; ++i)
	{
		MeshPart* part = at(i);
		// 写入Part
		// index
		// material? mode??
	}
}

Model::Model()
{

}

Model::~Model()
{

}

bool Model::load(Stream* stream)
{
	uint32 magic;
	uint8 version;
	uint8 tag;
	if (!stream->readMagic(magic) || magic != MODEL_MAGIC)
		return false;
	if (!stream->read(version) || version != MODEL_VERSION)
		return false;
	while (!stream->eof())
	{
		if (!stream->read(tag))
			return false;
		if (tag == ENDS_TAG)
			break;
		switch (tag)
		{
		case MESH_TAG:
		{
			m_mesh = new Mesh();
			if (!m_mesh->load(stream))
				return false;
		}
		break;
		case BONE_TAG:
		{
			m_skeleton = new Skeleton();
			if (!m_skeleton->load(stream))
				return false;
		}
		break;
		case ANIM_TAG:
		{
			m_animation = new Animation();
			if (!m_animation->load(stream))
				return false;
		}
		break;
		case MESH_LINK:
		{
			String path;
			if (!stream->read(path))
				return false;
			Model* model = AssetCache::load<Model>(path);
			if (!model || !model->m_mesh)
				return false;
			m_mesh = model->m_mesh;
			m_mesh->retain();
		}
		break;
		case BONE_LINK:
		{
			String path;
			if (!stream->read(path))
				return false;
			Model* model = AssetCache::load<Model>(path);
			if (!model || !model->m_skeleton)
				return false;
			m_skeleton = model->m_skeleton;
			m_skeleton->retain();
		}
		break;
		case ANIM_LINK:
		{
			String path;
			if (!stream->read(path))
				return false;
			Model* model = AssetCache::load<Model>(path);
			if (!model || !model->m_animation)
				return false;
			m_animation = model->m_animation;
			m_animation->retain();
		}
		break;
		}
	}
	// 加载flag
	return true;
}

void Model::save(Stream* stream)
{
	stream->write(MODEL_MAGIC);
	// 版本信息
	stream->write(MODEL_VERSION);
	// mesh
	if (m_mesh)
	{
		stream->write(MESH_TAG);
		m_mesh->save(stream);
	}

	// skeleton
	if (m_skeleton)
	{
		stream->write(BONE_TAG);
		m_skeleton->save(stream);
	}

	// animation
	if (m_animation)
	{
		stream->write(ANIM_TAG);
		// 序列化animation
		m_animation->save(stream);
	}
	stream->write(ENDS_TAG);
}

CU_NS_END
