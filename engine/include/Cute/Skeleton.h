#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector3.h"
#include "Cute/Matrix4.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Bone
{
public:
private:
    typedef Vector<Bone*> BoneArray;
    String      m_name;
    Matrix4     m_pose;
    Bone*       m_parent;
    BoneArray   m_children;
};

class CUTE_CORE_API Skeleton
{
public:
    Skeleton();
    ~Skeleton();
    
    void setBindShape(const Matrix4& shape);
    void addBone();
    
    const Bone* getBone(uint index) const;
    const Bone* getBone(const String& name) const;
    
private:
    typedef Vector<Bone*>       BoneVec;
    typedef Map<String, Bone*>  BoneMap;
    BoneVec m_bones;
    BoneMap m_names;
};

CUTE_NS_END
