#include "XMLFile.h"
#include "File.h"
#include "xml/pugixml.hpp"
using namespace pugi;

CU_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

XMLNode::XMLNode(void* node)
	: m_node(node)
{
}

XMLNode::~XMLNode()
{
}

XMLNode XMLNode::getParent() const
{
	if (!m_node)
		return XMLNode();
	xml_node node((xml_node_struct*)m_node);
	return XMLNode(node.parent().internal_object());
}

XMLNode XMLNode::getNext(const String& name) const
{
	if (!m_node)
		return XMLNode();
	xml_node node((xml_node_struct*)m_node);
	if (!name.empty())
		return XMLNode(node.next_sibling(name.c_str()).internal_object());
	else
		return XMLNode(node.next_sibling().internal_object());
}

XMLNode XMLNode::createChild(const String& name)
{
	xml_node node((xml_node_struct*)m_node);
	xml_node child = node.append_child(name.c_str());
	return XMLNode(child.internal_object());
}

XMLNode XMLNode::getChild(const String& name) const
{
	xml_node node((xml_node_struct*)m_node);
	if (!name.empty())
		return XMLNode(node.child(name.c_str()).internal_object());
	else
		return XMLNode(node.first_child().internal_object());
}

bool XMLNode::hasChild(const String& name) const
{
	xml_node node((xml_node_struct*)m_node);
	return !node.child(name.c_str()).empty();
}

bool XMLNode::removeChild(const String& name)
{
	xml_node node((xml_node_struct*)m_node);
	return node.remove_child(name.c_str());
}

bool XMLNode::removeChildren(const String& name)
{
	if (!m_node)
		return false;

	xml_node node((xml_node_struct*)m_node);
	if (!name.empty())
	{
		// 删除同名字的
		for (xml_node::iterator itor = node.begin(); itor != node.end();)
		{
			if (strcmp(itor->name(), name.c_str()) == 0)
			{
				node.remove_child(*itor++);
			}
			else
			{
				++itor;
			}
		}
	}
	else
	{
		// 删除全部
		for (xml_node::iterator itor = node.begin(); itor != node.end();)
		{
			node.remove_child(*itor++);
		}
	}
	return true;
}

bool XMLNode::removeAttribute(const String& name)
{
	xml_node node((xml_node_struct*)m_node);
	return node.remove_attribute(name.c_str());
}

bool XMLNode::setValue(const String& value)
{
	xml_node node((xml_node_struct*)m_node);
	return node.set_value(value.c_str());
}

bool XMLNode::setAttribute(const String& key, const String& value)
{
	xml_node node((xml_node_struct*)m_node);
	xml_attribute attr = node.attribute(key.c_str());
	if (attr.empty())
		attr = node.append_attribute(key.c_str());
	return attr.set_value(value.c_str());
}

bool XMLNode::hasAttribute(const String& key) const
{
	xml_node node((xml_node_struct*)m_node);
	return !node.attribute(key.c_str()).empty();
}

bool XMLNode::isNull() const
{
	return m_node == 0;
}

String XMLNode::getName() const
{
	xml_node node((xml_node_struct*)m_node);
	return String(node.name());
}

String XMLNode::getValue() const
{
	xml_node node((xml_node_struct*)m_node);
	return String(node.value());
}

String XMLNode::getAttribute(const String& key) const
{
	xml_node node((xml_node_struct*)m_node);
	return node.attribute(key.c_str()).value();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
XMLFile::XMLFile()
	: m_document(new xml_document())
{

}

XMLFile::XMLFile(const String& path)
	: m_document(new xml_document())
{
	open(path);
}

XMLFile::~XMLFile()
{
	delete (xml_document*)m_document;
	m_document = 0;
}

bool XMLFile::open(const String& path)
{
	xml_document* doc = (xml_document*)m_document;
	return doc->load_file(path.c_str());
}

bool XMLFile::save(const String& path) const
{
	xml_document* doc = (xml_document*)m_document;
	return doc->save_file(path.c_str());
}

XMLNode XMLFile::getRoot()
{
	xml_node root = ((xml_document*)m_document)->first_child();
	if (root.empty())
		return XMLNode();
	return XMLNode((void*)root.internal_object());
}

CU_NS_END
