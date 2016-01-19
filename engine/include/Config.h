#pragma once
#include "API.h"

CU_NS_BEGIN

class CU_API Setting
{
public:
	enum Type
	{
		T_NIL,	// null
		T_BOL,	// bool
		T_INT,	// int64
		T_NUM,	// double
		T_STR,	// str
		T_VEC,	// array
		T_OBJ,	// map table
	};
	Setting(Type type = T_NIL);
	~Setting();

	Setting(const Setting& other);
	Setting& operator=(const Setting& other);
	Setting& operator=(bool val);
	Setting& operator=(int64_t val);
	Setting& operator=(uint64_t val);
	Setting& operator=(double val);
	Setting& operator=(const String& val);

	void setObject();
	void setArray();
	void swap(Setting& other);

	bool   asBool() const { return m_bval; }
	sint64 asInt() const { return m_ival; }
	uint64 asUInt() const { return (uint64)m_ival; }
	float  asFloat() const { return (float)m_fval; }
	double asDouble() const { return m_fval; }
	String asString() const { return m_pstr ? m_pstr->data : ""; }
	const char* asCStr() const { return m_pstr ? m_pstr->data.c_str() : 0; }

	bool isNull() const { return m_type == T_NIL; }
	bool isBool() const { return m_type == T_BOL; }
	bool isInteger()const { return m_type == T_INT; }
	bool isDouble()	const { return m_type == T_NUM; }
	bool isString() const { return m_type == T_STR; }
	bool isArray()  const { return m_type == T_VEC; }
	bool isObject() const { return m_type == T_OBJ; }
	bool isIntegral() const { return m_type == T_BOL || m_type == T_INT; }
	bool isNumeric() const { return isIntegral() || isDouble(); }

	size_t size() const;
	bool empty() const;
	void clear();
	void resize(size_t len);
	void append(const Setting& value);
	bool contain(const String& key) const;

	Setting& get(size_t index);
	Setting& get(const String& key);

	const Setting& get(size_t index) const;
	const Setting& get(const String& key) const;

	Setting& operator[](size_t index) { return get(index); }
	Setting& operator[](const String& key) { return get(key); }

	const Setting& operator[](size_t index) const { return get(index); }
	const Setting& operator[](const String& key) const { return get(key); }

	String toString() const;
private:
	//String toString(Setting& value, std::stringstream ss, int level) const;
	void copy(const Setting& other);
	void release();

private:
	typedef std::vector<Setting>		SettingVec;
	typedef std::map<String, Setting>	SettingMap;
	struct ref_t
	{
		int8_t refs;
		ref_t() :refs(1){}
	};
	struct str_t : public ref_t
	{
		String data;
	};
	
	struct vec_t : public ref_t
	{
		SettingVec data;
	};

	struct map_t : public ref_t
	{
		SettingMap data;
	};

	Type m_type : 8;
	union
	{
		bool	m_bval;
		int64_t	m_ival;
		double	m_fval;
		str_t*	m_pstr;
		vec_t*	m_pvec;
		map_t*	m_pmap;
	};
};

/*
规则：类似json但是没有那么严格,like libconfig
1:root不要求必须含{},但是必须匹配
2:string不要求必须含""，但是有歧义的必须含有引号,即非纯数字，非符号,且不能是true，false，和null
3:obj分割符可以是：或者=
4:不强制要求最后一个分隔符,可含或者不含逗号
*/
class CU_API Config : public Setting
{
public:
	Config();
	~Config();

	bool load(const String& path);
	bool parse(const String& data);
};

CU_NS_END