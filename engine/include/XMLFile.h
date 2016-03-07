#pragma once
#include "API.h"
#include "Convert.h"

CU_NS_BEGIN

//like <node attribute="attribute1"> value </node>
// todo:string convert to StringPiece??
class CU_API XMLNode
{
public:
	XMLNode(void* node = NULL);
	~XMLNode();

	XMLNode getParent() const;
	XMLNode getNext(const String& name) const;

	XMLNode createChild(const String& name);
	XMLNode getChild(const String& name) const;
	bool hasChild(const String& name) const;
	bool removeChild(const String& name);
	bool removeChildren(const String& name);
	bool removeAttribute(const String& name);

	bool setValue(const String& value);
	bool setAttribute(const String& key, const String& value);
	bool hasAttribute(const String& key) const;

	bool isNull() const;
	String getName() const;
	String getValue() const;
	String getAttribute(const String& key) const;

	template<typename T>
	void getAttribute(const String& key, T& value) const
	{
		String str = getAttribute(key);
		Convert::convertTo(value, str);
	}

	template<typename T>
	T getAttribute(const String& key) const
	{
		T ret;
		getAttribute(key, ret);
		return ret;
	}

	template<typename T>
	bool setAttribute(const String& key, const T& value)
	{
		String str = Convert::toString(value);
		return setAttribute(key, str);
	}

	operator bool() const { return !isNull(); }
	bool operator!() const { return isNull(); }

private:
	void*	 m_node;
};

class CU_API XMLFile
{
public:
	XMLFile();
	XMLFile(const String& path);
	~XMLFile();

	bool open(const String& path);
	bool save(const String& path) const;

	XMLNode getRoot();

private:
	void* m_document;
};

// ≤È—Ø
//class CU_API XPathResult
//{
//public:
//	XPathResult();
//	~XPathResult();
//
//private:
//
//};
//
//// ≤È—Ø
//class CU_API XPathQuery
//{
//public:
//	XPathQuery();
//	~XPathQuery();
//
//private:
//	String m_query;
//};

CU_NS_END
