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
	Setting(Type type = T_NIL){}
	~Setting(){}

	void swap(Setting& other);

	const char* asString() const;
	sint64 asInt() const;
	uint64 asUInt() const;
	double asDouble() const;
	bool   asBool() const;

	bool isNull() const { return m_type == T_NIL; }
	bool isBool() const { return m_type == T_BOL; }
	bool isInteger()const { return m_type == T_INT; }
	bool isDouble()	const { return m_type == T_NUM; }
	bool isString() const { return m_type == T_STR; }
	bool isArray()  const { return m_type == T_VEC; }
	bool isObject() const { return m_type == T_OBJ; }
	bool isIntegral() const { return m_type == T_BOL || m_type == T_INT; }
	bool isNumeric() const { return isIntegral() || isDouble(); }

	bool contain(const char*) const;

	size_t size() const;
	bool empty() const;
	void clear();
	void resize(size_t len);
	void append(const Setting& value);

	Setting& get(size_t index);
	Setting& get(const char* key);

	Setting& operator[](size_t index);
	const Setting& operator[](size_t index) const;
	Setting& operator[](const char* key);
	const Setting& operator[](const char* key) const;
private:
	typedef std::vector<Setting>	ValueVec;
	typedef std::map<const char*, Setting>		ValueMap;
	union
	{
		bool	 bval;
		sint64	 ival;
		uint64	 uval;
		float64  fval;
		char*	 sval;
		ValueVec*	aval;
		ValueMap*	mval;
	};
	Type m_type : 8;
};

// config like json
class CU_API Config
{
public:
	Config();
	~Config();

	bool load(const String& path);
	bool parse(const String& data);
};

CU_NS_END