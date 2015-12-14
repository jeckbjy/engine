#pragma once
#include "Types.h"

CU_NS_BEGIN

enum VariantType
{
	VAR_NONE,
	VAR_INT,
	VAR_BOOL,
	VAR_FLOAT,
	VAR_VEC2,
	VAR_VEC3,
	VAR_VEC4,
	VAR_COLOR,
	VAR_QUATERNION,
	VAR_MAX,
	VAR_OBJ,	// 需要存储hash值
	VAR_ARRAY,
};

class Object;

// 枚举会强制转化成Int类型
// use rtti
class CU_API Variant
{
public:
	Variant();
	~Variant();

	String toString() const;

private:
	char isObj : 1;
	int type;
	int size;
	union
	{
		int		ivalue;
		float	fvalue;
		Object*	obj;
	};
};

CU_NS_END