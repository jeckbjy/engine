#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector3.h"
#include "Cute/Matrix4.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Bone
{
public:
public:
    String  m_name;
    Vector3 m_pos;
};

class CUTE_CORE_API Skeleton
{
public:
    Skeleton();
    ~Skeleton();
    
private:
    typedef Vector<Bone> BoneList;
    BoneList m_bones;
};

CUTE_NS_END
