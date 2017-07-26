// module Scene
#include "Cute/SceneNode.h"
#include "Cute/Transform.h"

CUTE_NS_BEGIN

SceneNode::SceneNode()
    : m_parent(NULL)
{
    addComponent(new Transform());
}

SceneNode::~SceneNode()
{

}

void SceneNode::addChild(SceneNode *node)
{
    // retain ref?
    m_children.push_back(node);
}

SceneNode* SceneNode::getChild(const String &name, bool recursice) const
{
    foreach(SceneNode* node, m_children)
    {
        if(node->getName() == name)
            return node;
    }
    
    if(recursice)
    {
        foreach(SceneNode* child, m_children)
        {
            SceneNode* node = child->getChild(name, true);
            if(node != NULL)
                return node;
        }
    }
    
    return NULL;
}

bool SceneNode::isChildOf(SceneNode *node) const
{
    foreach(SceneNode* temp, m_children)
    {
        if(temp == node)
            return true;
    }
    
    return false;
}

long SceneNode::indexOfChild(SceneNode *node) const
{
    for(size_t i = 0; i < m_children.size(); ++i)
    {
        if(m_children[i] == node)
            return (long)i;
    }
    
    return -1;
}

Transform* SceneNode::getTransform() const
{
//    return (Transform*)m_components[0];
    return m_components[0]->cast<Transform>();
}

CUTE_NS_END
