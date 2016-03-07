#pragma once
#include "API.h"

CU_NS_BEGIN

// 提供string类型转换
class CU_API Convert
{
public:
	// convert string to value
	static void convertTo(bool&		value, const String& str) { value = atoi(str.c_str()) != 0; }
	static void convertTo(int8_t&	value, const String& str) { value = (int8_t)atoi(str.c_str()); }
	static void convertTo(uint8_t&	value, const String& str) { value = (uint8_t)atoi(str.c_str()); }
	static void convertTo(int16_t&	value, const String& str) { value = (int16_t)atoi(str.c_str()); }
	static void convertTo(uint16_t& value, const String& str) { value = (uint16_t)atoi(str.c_str()); }
	static void convertTo(int32_t&	value, const String& str) { value = (int32_t)atoi(str.c_str()); }
	static void convertTo(uint32_t& value, const String& str) { value = (uint32_t)atoi(str.c_str()); }
	static void convertTo(int64_t&	value, const String& str) { value = (int64_t)atoll(str.c_str()); }
	static void convertTo(uint64_t& value, const String& str) { value = (uint64_t)atoll(str.c_str()); }
	static void convertTo(float&	value, const String& str) { value = (float)atof(str.c_str()); }
	static void convertTo(double&	value, const String& str) { value = (double)atof(str.c_str()); }

	static void convertTo(Vector2& value, const String& str);
	static void convertTo(Vector3& value, const String& str);
	static void convertTo(Vector4& value, const String& str);
	static void convertTo(Matrix3& value, const String& str);
	static void convertTo(Matrix4& value, const String& str);
	static void convertTo(Matrix3x4& value, const String& str);
	static void convertTo(Quaternion& value, const String& str);
	static void convertTo(Color& value, const String& str);

	// convert value to string
	// basic
	static void convertFrom(String& str, bool value)	{ convert_basic<int>(str, value); }
	static void convertFrom(String& str, int8_t value)	{ convert_basic(str, value); }
	static void convertFrom(String& str, uint8_t value) { convert_basic(str, value); }
	static void convertFrom(String& str, int16_t value) { convert_basic(str, value); }
	static void convertFrom(String& str, uint16_t value){ convert_basic(str, value); }
	static void convertFrom(String& str, int32_t value) { convert_basic(str, value); }
	static void convertFrom(String& str, uint32_t value){ convert_basic(str, value); }
	static void convertFrom(String& str, int64_t value) { convert_basic(str, value); }
	static void convertFrom(String& str, uint64_t value){ convert_basic(str, value); }
	static void convertFrom(String& str, float value)	{ convert_basic(str, value); }
	static void convertFrom(String& str, double value)	{ convert_basic(str, value); }

	static void convertFrom(String& str, const Vector2& value);
	static void convertFrom(String& str, const Vector3& value);
	static void convertFrom(String& str, const Vector4& value);
	static void convertFrom(String& str, const Matrix3& value);
	static void convertFrom(String& str, const Matrix4& value);
	static void convertFrom(String& str, const Matrix3x4& value);
	static void convertFrom(String& str, const Quaternion& value);
	static void convertFrom(String& str, const Color& value);

	template<typename T>
	static String toString(const T& value)
	{
		String str;
		convertFrom(str, value);
		return str;
	}

private:
	template<typename T>
	static void convert_basic(String& str, T value)
	{
		std::stringstream ss;
		ss << value;
		str = ss.str();
	}

	// 分割，并忽略空的数据
	static bool split(const String& str, float* values, size_t max, char delimiter = ' ');
};

CU_NS_END
