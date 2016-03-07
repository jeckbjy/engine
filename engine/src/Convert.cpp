#include "Convert.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Matrix3x4.h"
#include "Quaternion.h"
#include "Color.h"

CU_NS_BEGIN

bool Convert::split(const String& str, float* values, size_t max, char delimiter)
{
	assert(max > 0);
	size_t max_size = str.size();
	size_t count = 0;
	size_t index = 0;
	size_t end_index;
	while (index < max_size)
	{
		// 先查找起始
		while (str[index] == delimiter && index < max_size)
			++index;
		if (index == max_size)
			break;
		// 查找结束
		end_index = index + 1;
		while (str[end_index] != delimiter && end_index < max_size)
			++end_index;
		// 转化成float
		sscanf(str.c_str() + index, "%f", values[count]);
		++count;
		if (count == max)
			return true;
	}

	return false;
}

void Convert::convertTo(Vector2& value, const String& str)
{
	split(str, value.data(), 2);
}

void Convert::convertTo(Vector3& value, const String& str)
{
	split(str, value.data(), 3);
}

void Convert::convertTo(Vector4& value, const String& str)
{
	split(str, value.data(), 4);
}

void Convert::convertTo(Matrix3& value, const String& str)
{
	split(str, value.data(), 9);
}

void Convert::convertTo(Matrix4& value, const String& str)
{
	split(str, value.data(), 16);
}

void Convert::convertTo(Matrix3x4& value, const String& str)
{
	split(str, value.data(), 12);
}

void Convert::convertTo(Quaternion& value, const String& str)
{
	split(str, value.data(), 4);
}

void Convert::convertTo(Color& value, const String& str)
{
	split(str, value.data(), 4);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void Convert::convertFrom(String& str, const Vector2& value)
{
	str = value.toString();
}

void Convert::convertFrom(String& str, const Vector3& value)
{
	str = value.toString();
}

void Convert::convertFrom(String& str, const Vector4& value)
{
	str = value.toString();
}

void Convert::convertFrom(String& str, const Matrix3& value)
{
	str = value.toString();
}

void Convert::convertFrom(String& str, const Matrix4& value)
{
	str = value.toString();
}

void Convert::convertFrom(String& str, const Matrix3x4& value)
{
	str = value.toString();
}

void Convert::convertFrom(String& str, const Quaternion& value)
{
	str = value.toString();
}

void Convert::convertFrom(String& str, const Color& value)
{
	str = value.toString();
}

CU_NS_END
