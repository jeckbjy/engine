#pragma once
#include "Cute/Object.h"
#include "Cute/Entity.h"
//#include "Cute/List.h"

CUTE_NS_BEGIN

class CUTE_CORE_API SceneNode : public TObject<SceneNode>, public Entity
{
public:
    static SceneNode* create(const String& name, int flags = 0);
    
	SceneNode();
    SceneNode(const String& name);
	~SceneNode();
    
    const String&   getName() const;
    SceneNode*      getParent() const;
    SceneNode*      getChild(uint index) const;
    SceneNode*      getChild(const String& name, bool recursice = false) const;
    void            addChild(SceneNode* node);
    
    bool            isChildOf(SceneNode* node) const;
    long            indexOfChild(SceneNode* node) const;
    uint            getNumChildren() const;
    
    Transform*      getTransform() const;
    
    template<typename Func>
    void traverseChildren(Func func);

protected:
    typedef Vector<SceneNode*>      NodeList;
    typedef NodeList::iterator      NodeIterator;
    typedef NodeList::const_pointer NodeConstIterator;
    
    String      m_name;
    SceneNode*  m_parent;
    NodeList    m_children;
};

//
// inlines

inline const String& SceneNode::getName() const
{
    return m_name;
}

inline SceneNode* SceneNode::getParent() const
{
    return m_parent;
}

inline uint SceneNode::getNumChildren() const
{
    return m_children.size();
}

inline SceneNode* SceneNode::getChild(uint index) const
{
    return m_children[index];
}

template<typename Func>
void SceneNode::traverseChildren(Func func)
{
    foreach(SceneNode* node, m_children)
    {
        func(node);
    }
}

CUTE_NS_END
