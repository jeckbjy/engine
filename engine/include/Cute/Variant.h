#pragma once
#include "Cute/Foundation.h"
#include "Cute/DateTime.h"
#include "Cute/Ref.h"

CUTE_NS_BEGIN

class CUTE_CORE_API VHolder : public Ref<VHolder>
{
public:
	VHolder(){}
	virtual ~VHolder(){}

	virtual VHolder* clone() const = 0;
	virtual const std::type_info& type() const = 0;

	virtual void encode(String& value) const;
	virtual void decode(const String& value);
	virtual void clear();
	virtual void resize(size_t capacity);
	virtual bool equal(VHolder* holder) const;
	virtual size_t size() const;
};

class VArray;
class VObject;
class VString;
class VDateTime;
class CUTE_CORE_API Variant
{
public:
	enum VarType
	{
		VAR_NULL,
		VAR_INT,
		VAR_UINT,
		VAR_REAL,
		VAR_BOOL,
		VAR_DATE,
		VAR_STR,
		VAR_ARRAY,
		VAR_OBJ,
		VAR_ANY,
	};

	static const Variant null;

	typedef std::map<String, Variant>	ObjectMap;
	typedef ObjectMap::iterator			Iterator;
	typedef ObjectMap::const_iterator	ConstIterator;
public:
	Variant();
	Variant(VarType type);
	Variant(bool value);
	Variant(int value);
	Variant(int64 value);
	Variant(uint64 value);
	Variant(float value);
	Variant(double value);
	Variant(const char* value);
	Variant(const String& value);
	Variant(const DateTime& date);
	Variant(const Variant& other);
	~Variant();

	Variant& operator=(bool   value);
	Variant& operator=(int    value);
	Variant& operator=(int64  value);
	Variant& operator=(uint64 value);
	Variant& operator=(float  value);
	Variant& operator=(double value);
	Variant& operator=(const char*   value);
	Variant& operator=(const String&   value);
	Variant& operator=(const DateTime& value);
	Variant& operator=(const Variant& other);

	void swap(Variant& other);
	void reset();

	bool isNull() const;
	bool isBool() const;
	bool isSigned() const;
	bool isUnsigned() const;
	bool isInteger() const;
	bool isDouble() const;
	bool isNumeric() const;
	bool isString() const;
	bool isArray() const;
	bool isObject() const;
	bool isPointer() const;

	bool isMember(const String& key) const;
	void erase(const String& key);
	void erase(size_t index);
	
	bool empty() const;
	void clear();
	void resize(size_t capacity);
	size_t size() const;
	VarType type() const;

	int		asInt() const;
	uint	asUInt() const;
	int64	asInt64() const;
	uint64	asUInt64() const;
	float	asFloat() const;
	double	asDouble() const;
	bool	asBool() const;
	String	asString() const;

	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

	Variant& append(const Variant& value);

	Variant&		operator[](size_t index);
	const Variant&	operator[](size_t index) const;
	Variant&		operator[](const String& key);
	const Variant&	operator[](const String& key) const;

	bool operator < (const Variant& other) const;
	bool operator <=(const Variant& other) const;
	bool operator >=(const Variant& other) const;
	bool operator > (const Variant& other) const;

	bool operator ==(const Variant& other) const;
	bool operator !=(const Variant& other) const;

private:
	void construct(VarType type);
	void destruct();
	void copy(const Variant& other);

private:
	union
	{
		bool		m_bool;
		int64		m_int;
		uint64		m_uint;
		double		m_real;
		VDateTime*	m_date;
		VString*	m_str;
		VArray*		m_arr;
		VObject*	m_obj;
		VHolder*	m_any;
	};

	VarType m_type;
};

CUTE_NS_END
