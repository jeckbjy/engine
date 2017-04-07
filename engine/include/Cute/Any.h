#pragma once
#include "Cute/Foundation.h"
#include <algorithm>
#include <typeinfo>
#include <cstring>

CUTE_NS_BEGIN

/// An Any class represents a general type and is capable of storing any type, supporting type-safe extraction
/// of the internally stored data.
///
/// Code taken from the Boost 1.33.1 library. Original copyright by Kevlin Henney. Modified by Applied Informatics
///
/// Modified for small object optimization support (optionally supported through conditional compilation)
/// by Alex Fabijanic.
/// Example: 
///	 Any a(13); 
///	 Any a(string("12345"));
class Any
{
public:
	Any()
		: m_holder(0)
	{
	}

	template<typename T>
	Any(const T& value)
		: m_holder(new Holder<T>(value))
	{
	}

	Any(const Any& other)
		: m_holder(other.m_holder ? other.m_holder->clone() : 0)
	{
	}

	~Any()
	{
		delete m_holder;
	}

	Any& swap(Any& other)
	{
		std::swap(m_holder, other.m_holder);
		return *this;
	}

	template<typename T>
	Any& operator = (const T& other)
	{
		Any(other).swap(*this);
		return *this;
	}

	Any& operator = (const T& other)
	{
		Any(other).swap(*this);
		return *this;
	}

	bool empty() const
	{
		return !m_holder;
	}

	const std::type_info& type() const
	{
		return m_holder ? m_holder->type() : typeid(void);
	}

private:
	class ValueHolder
	{
	public:
		virtual ~ValueHolder(){}
		virtual const std::type_info& type() = 0;
		virtual ValueHolder* clone() const = 0;
	};

	template<typename ValueType>
	class Holder : public ValueHolder
	{
	public:
		Holder(const ValueType& value):m_held(value){}

		virtual const std::type_info& type() const
		{
			return typeid(ValueType);
		}

		virtual ValueHolder* clone() const
		{
			return new Holder(m_held);
		}

	private:
		Holder& operator=(const Holder&);
		ValueType m_held;
	};

	template<typename T>
	T* cast() const
	{
		return m_holder ? &(static_cast<Holder<T>*>(m_holder)->m_held): NULL;
	}

private:
	ValueHolder* m_holder;
};

CUTE_NS_END
