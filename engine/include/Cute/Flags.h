#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

template<typename Enum, typename Storage = uint32>
class Flags
{
public:

	Flags()
		:mBits(0)
	{ }

	Flags(Enum value)
	{
		mBits = static_cast<Storage>(value);
	}

	Flags(const Flags<Enum, Storage>& value)
	{
		mBits = value.mBits;
	}

	explicit Flags(Storage bits)
	{
		mBits = bits;
	}

	/** Checks whether all of the provided bits are set */
	bool isSet(Enum value) const
	{
		return (mBits & static_cast<Storage>(value)) == static_cast<Storage>(value);
	}

	/** Activates all of the provided bits. */
	Flags<Enum, Storage>& set(Enum value)
	{
		mBits = static_cast<Storage>(value);

		return *this;
	}

	/** Deactivates all of the provided bits. */
	void unset(Enum value)
	{
		mBits &= ~static_cast<Storage>(value);
	}

	bool operator==(Enum rhs) const
	{
		return mBits == static_cast<Storage>(rhs);
	}

	bool operator==(const Flags<Enum, Storage>& rhs) const
	{
		return mBits == rhs.mBits;
	}

	bool operator==(bool rhs) const
	{
		return ((bool)*this) == rhs;
	}

	bool operator!=(Enum rhs) const
	{
		return mBits != static_cast<Storage>(rhs);
	}

	bool operator!=(const Flags<Enum, Storage>& rhs) const
	{
		return mBits != rhs.mBits;
	}

	Flags<Enum, Storage>& operator= (Enum rhs)
	{
		mBits = static_cast<Storage>(rhs);

		return *this;
	}

	Flags<Enum, Storage>& operator= (const Flags<Enum, Storage>& rhs)
	{
		mBits = rhs.mBits;

		return *this;
	}

	Flags<Enum, Storage>& operator|= (Enum rhs)
	{
		mBits |= static_cast<Storage>(rhs);

		return *this;
	}

	Flags<Enum, Storage>& operator|= (const Flags<Enum, Storage>& rhs)
	{
		mBits |= rhs.mBits;

		return *this;
	}

	Flags<Enum, Storage> operator| (Enum rhs) const
	{
		Flags<Enum, Storage> out(*this);
		out |= rhs;

		return out;
	}

	Flags<Enum, Storage> operator| (const Flags<Enum, Storage>& rhs) const
	{
		Flags<Enum, Storage> out(*this);
		out |= rhs;

		return out;
	}

	Flags<Enum, Storage>& operator&= (Enum rhs)
	{
		mBits &= static_cast<Storage>(rhs);

		return *this;
	}

	Flags<Enum, Storage>& operator&= (const Flags<Enum, Storage>& rhs)
	{
		mBits &= rhs.mBits;

		return *this;
	}

	Flags<Enum, Storage> operator& (Enum rhs) const
	{
		Flags<Enum, Storage> out = *this;
		out.mBits &= static_cast<Storage>(rhs);

		return out;
	}

	Flags<Enum, Storage> operator& (const Flags<Enum, Storage>& rhs) const
	{
		Flags<Enum, Storage> out = *this;
		out.mBits &= rhs.mBits;

		return out;
	}

	Flags<Enum, Storage>& operator^= (Enum rhs)
	{
		mBits ^= static_cast<Storage>(rhs);

		return *this;
	}

	Flags<Enum, Storage>& operator^= (const Flags<Enum, Storage>& rhs)
	{
		mBits ^= rhs.mBits;

		return *this;
	}

	Flags<Enum, Storage> operator^ (Enum rhs) const
	{
		Flags<Enum, Storage> out = *this;
		out.mBits ^= static_cast<Storage>(rhs);

		return out;
	}

	Flags<Enum, Storage> operator^ (const Flags<Enum, Storage>& rhs) const
	{
		Flags<Enum, Storage> out = *this;
		out.mBits ^= rhs.mBits;

		return out;
	}

	Flags<Enum, Storage> operator~ () const
	{
		Flags<Enum, Storage> out;
		out.mBits = (Storage)~mBits;

		return out;
	}

	operator bool() const
	{
		return mBits ? true : false;
	}

	operator uint8() const
	{
		return static_cast<uint8>(mBits);
	}

	operator uint16() const
	{
		return static_cast<uint16>(mBits);
	}

	operator uint32() const
	{
		return static_cast<uint32>(mBits);
	}

	friend Flags<Enum, Storage> operator&(Enum a, Flags<Enum, Storage> &b)
	{
		Flags<Enum, Storage> out;
		out.mBits = a & b.mBits;
		return out;
	}

private:
	Storage mBits;
};

/** Defines global operators for a Flags<Enum, Storage> implementation. */
#define CUTE_FLAGS_OPERATORS(Enum) CUTE_FLAGS_OPERATORS_EXT(Enum, uint32)

/** Defines global operators for a Flags<Enum, Storage> implementation. */
#define CUTE_FLAGS_OPERATORS_EXT(Enum, Storage)																   \
		inline Flags<Enum, Storage> operator|(Enum a, Enum b) { Flags<Enum, Storage> r(a); r |= b; return r; } \
		inline Flags<Enum, Storage> operator&(Enum a, Enum b) { Flags<Enum, Storage> r(a); r &= b; return r; } \
		inline Flags<Enum, Storage> operator~(Enum a) { return ~Flags<Enum, Storage>(a); }

CUTE_NS_END
