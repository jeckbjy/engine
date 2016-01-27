#pragma once
#include "Asset.h"
#include "Skeleton.h"

CU_NS_BEGIN

// 模型数据
class CU_API Model : public Asset
{
public:
	Model();
	~Model();

private:
	Skeleton* m_skeleton;
};

CU_NS_END