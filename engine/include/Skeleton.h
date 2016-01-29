#pragma once
#include "API.h"
#include "List.h"
#include "Matrix4.h"
#include "Stream.h"

CU_NS_BEGIN

// ¹Ç÷ÀÐÅÏ¢
struct Bone : public ListNode<Bone>
{
	typedef List<Bone> BoneList;
	uint	index;
	String	name;
	Matrix4	pose;
	Bone*	parent;
	BoneList children;
};

class CU_API Skeleton : public Ref
{
public:
	Skeleton();
	~Skeleton();

	bool load(Stream* stream);
	void save(Stream* stream);

	void addBone(Bone* bone);

	const Bone* getBone(const String& name) const;
	const Bone* getBone(size_t index) const { return m_bones[index]; }
	size_t getBoneCount() const { return m_bones.size(); }
	bool hasBone(const String& name) const { return getBone(name) != NULL; }

private:
	typedef Vector<Bone*> BoneVec;
	typedef std::map<String, Bone*> BoneMap;
	Matrix4* m_shape;
	BoneVec	 m_bones;
	BoneMap	 m_names;
};

CU_NS_END
