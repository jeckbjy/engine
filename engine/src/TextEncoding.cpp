// module Text
#include "Cute/TextEncoding.h"
#include "Cute/UTF8Encoding.h"
#include "Cute/UTF16Encoding.h"
#include "Cute/UTF32Encoding.h"

#include "Cute/RWLock.h"
#include "Cute/String.h"

CUTE_NS_BEGIN

class TextEncodingManager
{
public:
	TextEncodingManager& instance()
	{
		static TextEncodingManager s_instance;
		return s_instance;
	}

public:
	TextEncodingManager()
	{
		add(new UTF8Encoding());
		add(new UTF16Encoding());
		add(new UTF32Encoding());
	}

	~TextEncodingManager()
	{
	}

	void add(TextEncoding* encoding)
	{
		add(encoding, encoding->canonicalName());
	}

	void add(TextEncoding* encoding, const String& name)
	{
		RWLock::ScopedLock lock(m_lock, true);

		m_encodings[name] = encoding;
	}

	void remove(const String& name)
	{
		RWLock::ScopedLock lock(m_lock, true);

		m_encodings.erase(name);
	}

	TextEncoding* find(const String& name) const
	{
		RWLock::ScopedLock lock(m_lock);

		EncodingMap::const_iterator it = m_encodings.find(name);
		if (it != m_encodings.end())
			return it->second;

		for (it = m_encodings.begin(); it != m_encodings.end(); ++it)
		{
			if (it->second->isA(name))
				return it->second;
		}

		return NULL;
	}

private:
	typedef std::map<std::string, TextEncoding*, CILess> EncodingMap;
	EncodingMap		m_encodings;
	mutable RWLock	m_lock;
};

//////////////////////////////////////////////////////////////////////////
// TextEncoding
//////////////////////////////////////////////////////////////////////////
TextEncoding::~TextEncoding()
{
}

int TextEncoding::convert(const unsigned char* bytes) const
{
	return static_cast<int>(*bytes);
}

int TextEncoding::convert(int ch, unsigned char* bytes, int length) const
{
	return 0;
}

int TextEncoding::queryConvert(const unsigned char* bytes, int length) const
{
	return (int)*bytes;
}

int TextEncoding::sequenceLength(const unsigned char* bytes, int length) const
{
	return 1;
}

//////////////////////////////////////////////////////////////////////////
// static
//////////////////////////////////////////////////////////////////////////

CUTE_NS_END
