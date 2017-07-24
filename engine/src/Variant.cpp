//! Dynamic
#include "Cute/Variant.h"
#include "Cute/Exception.h"
#include "Cute/Number.h"
#include "Cute/DateTime.h"
#include "Cute/Timestamp.h"

CUTE_NS_BEGIN

void VHolder::encode(String& value) const
{
	throw BadCastException("Can not format");
}

void VHolder::decode(const String& value)
{
	throw BadCastException("Can not parse");
}

size_t VHolder::size() const
{
	return 0;
}

bool VHolder::equal(VHolder* holder) const
{
	return false;
}

void VHolder::clear()
{
}

void VHolder::resize(size_t capacity)
{
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class VArray : public VHolder
{
public:
	typedef std::vector<Variant> Container;

	VArray(){}
	VArray(const Container& values) : m_values(values){}
	~VArray(){}

	VHolder* clone() const
	{
		return new VArray(m_values);
	}

	const std::type_info& type() const
	{
		return typeid(VArray);
	}

	Variant& get(size_t index)
	{
		return m_values[index];
	}

	void erase(size_t index)
	{
		m_values.erase(m_values.begin() + index);
	}

	size_t size() const 
	{
		return m_values.size(); 
	}

	bool equal(VHolder* holder) const
	{
		return m_values == ((VArray*)holder)->m_values;
	}

	Variant& append(const Variant& other)
	{
		m_values.push_back(other);
		return m_values.back();
	}

private:
	Container m_values;
};

class VObject : public VHolder
{
public:
	typedef std::map<String, Variant> Container;

	VObject(){}
	VObject(const Container& values) :m_values(values){}
	~VObject(){}

	VHolder* clone() const
	{
		return new VObject(m_values);
	}

	const std::type_info& type() const
	{
		return typeid(VObject);
	}

	bool has(const String& key) const
	{
		return m_values.find(key) != m_values.end();
	}

	void erase(const String& key)
	{
		m_values.erase(key);
	}

	Variant& get(const String& key)
	{
		return m_values[key];
	}

	const Variant& get(const String& key) const
	{
		Container::const_iterator itor = m_values.find(key);
		if (itor != m_values.end())
			return itor->second;

		throw InvalidArgumentException(key);
	}

	size_t size() const 
	{
		return m_values.size(); 
	}

	bool equal(VHolder* holder) const
	{
		return m_values == ((VObject*)holder)->m_values;
	}

	Container& value()
	{
		return m_values;
	}

private:
	Container m_values;
};

class VString : public VHolder
{
public:
	VString(){}
	VString(const String& str) :m_value(str){}
	~VString(){}

	VHolder* clone() const
	{
		return new VString(m_value);
	}

	const std::type_info& type() const
	{
		return typeid(VString);
	}

	void encode(String& val) const
	{
		val = m_value;
	}

	void decode(const String& val)
	{
		m_value = val;
	}

	size_t size() const 
	{
		return m_value.size(); 
	}

	String& value()
	{
		return m_value; 
	}

	bool equal(VHolder* holder) const
	{
		return m_value == ((VString*)holder)->m_value;
	}

private:
	String m_value;
};

class VDateTime : public VHolder
{
public:
	VDateTime(){}
	VDateTime(const DateTime& val) : m_value(val){}
	~VDateTime(){}

	VHolder* clone() const
	{
		return new VDateTime(m_value);
	}

	const std::type_info& type() const
	{
		return typeid(VDateTime);
	}

	void encode(String& value) const
	{
        DateTime::format(value, m_value);
	}

	void decode(const String& value)
	{
        if(!DateTime::parse(m_value, value))
            throw BadCastException("string -> DateTime");
	}

	DateTime& value()
	{
		return m_value;
	}

	bool equal(VHolder* holder) const
	{
		return m_value == ((VDateTime*)holder)->m_value;
	}

private:
	DateTime m_value;
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
const Variant Variant::null;

Variant::Variant()
	: m_type(VAR_NULL)
	, m_uint(0)
{
}

Variant::Variant(VarType type)
	: m_type(type)
{
	construct(type);
}

Variant::Variant(bool value)
	: m_type(VAR_BOOL)
	, m_bool(value)
{
}

Variant::Variant(int value)
	: m_type(VAR_INT)
	, m_int(value)
{
}

Variant::Variant(int64 value)
	: m_type(VAR_INT)
	, m_int(value)
{
}

Variant::Variant(uint64 value)
	: m_type(VAR_UINT)
	, m_uint(value)
{
}

Variant::Variant(float value)
	: m_type(VAR_REAL)
	, m_real(value)
{
}

Variant::Variant(double value)
	: m_type(VAR_REAL)
	, m_real(value)
{
}

Variant::Variant(const char* value)
	: m_type(VAR_STR)
	, m_str(new VString(value))
{
}

Variant::Variant(const String& value)
	: m_type(VAR_STR)
	, m_str(new VString(value))
{
}

Variant::Variant(const DateTime& date)
	: m_type(VAR_DATE)
	, m_date(new VDateTime(date))
{
}

Variant::Variant(const Variant& other)
{
	copy(other);
}

Variant::~Variant()
{
	destruct();
}

Variant& Variant::operator=(bool   value)
{
	destruct();
	m_type = VAR_BOOL;
	m_bool = value;
	return *this;
}

Variant& Variant::operator=(int    value)
{
	destruct();
	m_type = VAR_INT;
	m_int = value;
	return *this;
}

Variant& Variant::operator=(int64  value)
{
	destruct();
	m_type = VAR_INT;
	m_int = value;
	return *this;
}

Variant& Variant::operator=(uint64 value)
{
	destruct();
	m_type = VAR_UINT;
	m_uint = value;
	return *this;
}

Variant& Variant::operator=(float  value)
{
	destruct();
	m_type = VAR_REAL;
	m_real = value;
	return *this;
}

Variant& Variant::operator=(double value)
{
	destruct();
	m_type = VAR_REAL;
	m_real = value;
	return *this;
}

Variant& Variant::operator=(const char*   value)
{
	destruct();
	m_type = VAR_STR;
	m_str = new VString(value);
	return *this;
}

Variant& Variant::operator=(const String&   value)
{
	destruct();
	m_type = VAR_STR;
	m_str = new VString(value);
	return *this;
}
Variant& Variant::operator=(const DateTime& value)
{
	destruct();
	m_type = VAR_DATE;
	m_date = new VDateTime(value);
	return *this;
}

Variant& Variant::operator =(const Variant& other)
{
	if (&other != this)
	{
		destruct();
		copy(other);
	}
	return *this;
}

void Variant::swap(Variant& other)
{
	std::swap(m_uint, other.m_uint);
	std::swap(m_type, other.m_type);
}

void Variant::reset()
{
	destruct();
}

bool Variant::isNull() const
{
	return m_type == VAR_NULL;
}

bool Variant::isBool() const
{
	return m_type == VAR_BOOL;
}

bool Variant::isSigned() const
{
	return m_type == VAR_INT;
}

bool Variant::isInteger() const
{
	return m_type == VAR_INT || m_type == VAR_UINT;
}

bool Variant::isUnsigned() const
{
	return m_type == VAR_UINT;
}

bool Variant::isDouble() const
{
	return m_type == VAR_REAL;
}

bool Variant::isNumeric() const
{
	return m_type == VAR_INT || m_type == VAR_UINT || m_type == VAR_REAL;
}

bool Variant::isString() const
{
	return m_type == VAR_STR;
}

bool Variant::isArray() const
{
	return m_type == VAR_ARRAY;
}

bool Variant::isObject() const
{
	return m_type == VAR_OBJ;
}

bool Variant::isPointer() const
{
	return m_type >= VAR_DATE;
}

bool Variant::isMember(const String& key) const
{
	if (!isObject())
		return false;

	return m_obj->has(key);
}

Variant::VarType Variant::type() const
{
	return m_type;
}

void Variant::erase(const String& key)
{
	if (isObject())
	{
		m_obj->erase(key);
	}
}

void Variant::erase(size_t index)
{
	if (isArray())
	{
		m_arr->erase(index);
	}
}

bool Variant::empty() const
{
	if (isNull() || isArray() || isObject())
		return size() == 0;
	else
		return false;
}

size_t Variant::size() const
{
	if (isPointer())
	{
		return m_any ? m_any->size() : 0;
	}
	else
	{
		return 0;
	}
}

void Variant::clear()
{
	if (isPointer() && m_any != NULL)
	{
		m_any->clear();
	}
}

void Variant::resize(size_t capacity)
{
	if (isPointer() && m_any != NULL)
	{
		m_any->resize(capacity);
	}
}

void Variant::construct(VarType type)
{
	switch (type)
	{
	case VAR_DATE:
		m_date = new VDateTime();
		break;
	case VAR_STR:
		m_str = new VString();
		break;
	case VAR_ARRAY:
		m_arr = new VArray();
		break;
	case VAR_OBJ:
		m_obj = new VObject();
		break;
	default:
		m_uint = 0;
		break;
	}

	m_type = type;
}

void Variant::destruct()
{
	switch (m_type)
	{
	case VAR_DATE:
	case VAR_STR:
	case VAR_ARRAY:
	case VAR_OBJ:
	case VAR_ANY:
	{
		if (m_any)
		{
			m_any->release();
			m_any = NULL;
		}
		break;
	}
	default:
	{
		m_uint = 0;
		break;
	}
	}

	m_type = VAR_NULL;
}

void Variant::copy(const Variant& other)
{
	m_type = other.m_type;
	switch (m_type)
	{
	case VAR_INT:
		m_int = other.m_int;
		break;
	case VAR_UINT:
		m_uint = other.m_uint;
		break;
	case VAR_REAL:
		m_real = other.m_real;
		break;
	case VAR_BOOL:
		m_bool = other.m_bool;
		break;
	case VAR_DATE:
	case VAR_STR:
	case VAR_ARRAY:
	case VAR_OBJ:
	case VAR_ANY:
	{
		m_any = other.m_any;
		if (m_any)
			m_any->retain();
		break;
	}
	default:
		break;
	}
}

int Variant::asInt() const
{
	return (int)asInt64();
}

uint Variant::asUInt() const
{
	return (uint)asUInt64();
}

float Variant::asFloat() const
{
	return (float)asDouble();
}

int64 Variant::asInt64() const
{
	switch (m_type)
	{
	case VAR_NULL:
		return 0;
	case VAR_INT:
		return m_int;
	case VAR_UINT:
		return (int64)m_uint;
	case VAR_REAL:
		return (int64)m_real;
	case VAR_BOOL:
		return m_bool ? 1 : 0;
	case VAR_STR:
		return Number::parse64(m_str->value());
	default:
		return 0;
	}
}

uint64 Variant::asUInt64() const
{
	switch (m_type)
	{
	case VAR_NULL:
		return 0;
	case VAR_INT:
		return (uint64)m_int;
	case VAR_UINT:
		return (uint64)m_uint;
	case VAR_REAL:
		return (uint64)m_real;
	case VAR_BOOL:
		return m_bool ? 1 : 0;
	case VAR_STR:
		return Number::parseUnsigned64(m_str->value());
	default:
		return 0;
	}
}

double Variant::asDouble() const
{
	switch (m_type)
	{
	case VAR_NULL:
		return 0;
	case VAR_INT:
		return (double)m_int;
	case VAR_UINT:
		return (double)m_uint;
	case VAR_REAL:
		return (double)m_real;
	case VAR_BOOL:
		return m_bool ? 1 : 0;
	case VAR_STR:
		return Number::parseDouble(m_str->value());
	default:
		return 0;
	}
}

bool Variant::asBool() const
{
	switch (m_type)
	{
	case VAR_NULL:
		return false;
	case VAR_INT:
		return m_int != 0;
	case VAR_UINT:
		return m_uint != 0;
	case VAR_REAL:
		return m_real != 0;
	case VAR_BOOL:
		return m_bool;
	case VAR_STR:
		return Number::parseBool(m_str->value());
	default:
		return false;
	}
}

String Variant::asString() const
{
	switch (m_type)
	{
	case VAR_NULL:
		return "";
	case VAR_INT:
		return Number::format(m_int);
	case VAR_UINT:
		return Number::format(m_uint);
	case VAR_REAL:
		return Number::format(m_real);
	case VAR_BOOL:
		return Number::format(m_bool);
	default:
	{
		String val;
		m_any->encode(val);
		return val;
	}
	}
}

Variant::Iterator Variant::begin()
{
	if (m_type != VAR_OBJ)
		throw BadCastException("Variant begin");
	return m_obj->value().begin();
}

Variant::Iterator Variant::end()
{
	if (m_type != VAR_OBJ)
		throw BadCastException("Variant end");
	return m_obj->value().end();
}

Variant::ConstIterator Variant::begin() const
{
	if (m_type != VAR_OBJ)
		throw BadCastException("Variant begin");
	return m_obj->value().begin();
}

Variant::ConstIterator Variant::end() const
{
	if (m_type != VAR_OBJ)
		throw BadCastException("Variant end");
	return m_obj->value().end();
}

Variant& Variant::append(const Variant& value)
{
	if (isNull())
		construct(VAR_ARRAY);

	if (!isArray())
		throw BadCastException("Can not array");

	return m_arr->append(value);
}

Variant& Variant::operator [](size_t index)
{
	if (isNull())
		construct(VAR_ARRAY);

	if (!isArray())
		throw BadCastException("Can not array");

	return m_arr->get(index);
}

const Variant& Variant::operator [](size_t index) const
{
	if (isNull())
		return null;

	if (!isArray())
		throw BadCastException("array operator[]");

	if (index >= m_arr->size())
		throw BadCastException("array!");

	return m_arr->get(index);
}

Variant& Variant::operator [](const String& key)
{
	if (isNull())
		construct(VAR_OBJ);

	if (!isObject())
		throw BadCastException("object operator[]");

	return m_obj->get(key);
}

const Variant& Variant::operator [](const String& key) const
{
	if (isNull())
		return null;

	if (!isObject())
		throw BadCastException("object operator[]");

	return m_obj->get(key);
}

bool Variant::operator < (const Variant& other) const
{
	if (isNull())
		return false;

	if (m_type != other.m_type)
	{
		return asDouble() < other.asDouble();
	}
	else
	{
		switch (m_type)
		{
		case VAR_INT:
			return m_int < other.m_int;
		case VAR_UINT:
			return m_uint < other.m_uint;
		case VAR_REAL:
			return m_real < other.m_real;
		case VAR_BOOL:
			return m_bool < other.m_bool;
		case VAR_DATE:
			return m_date->value() < m_date->value();
		case VAR_STR:
			return m_str->value() < m_str->value();
		default:
			return false;
		}
	}

	return true;
}

bool Variant::operator <=(const Variant& other) const
{
	if (isNull())
		return false;

	if (m_type != other.m_type)
	{
		return asDouble() <= other.asDouble();
	}
	else
	{
		switch (m_type)
		{
		case VAR_INT:
			return m_int <= other.m_int;
		case VAR_UINT:
			return m_uint <= other.m_uint;
		case VAR_REAL:
			return m_real <= other.m_real;
		case VAR_BOOL:
			return m_bool <= other.m_bool;
		case VAR_DATE:
			return m_date->value() <= m_date->value();
		case VAR_STR:
			return m_str->value() <= m_str->value();
		default:
			return false;
		}
	}

	return true;
}

bool Variant::operator >=(const Variant& other) const
{
	return !(*this < other);
}

bool Variant::operator > (const Variant& other) const
{
	return !(*this <= other);
}

bool Variant::operator ==(const Variant& other) const
{
	//if ( type_ != other.type_ )
	// GCC 2.95.3 says:
	// attempt to take address of bit-field structure member `Json::Value::type_'
	// Beats me, but a temp solves the problem.
	int temp = other.m_type;
	if (m_type != temp)
		return false;

	switch (m_type)
	{
	case VAR_NULL:
		return true;
	case VAR_INT:
		return m_int == other.m_int;
	case VAR_UINT:
		return m_uint == other.m_uint;
	case VAR_REAL:
		return m_real == other.m_real;
	default:
	{
		if (m_any->type() != other.m_any->type())
			return false;
		return m_any->equal(other.m_any);
	}
	}
}

bool Variant::operator !=(const Variant& other) const
{
	return !(*this == other);
}

CUTE_NS_END
