//! Model
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// Mesh vs Geometry
// Renderable vs Drawable
//class CUTE_CORE_API Geometry
//{
//public:
//private:
//    typedef Vector<VertexBuffer*> VertexBufferList;
//    PrimitiveType       m_primitive;
//    VertexBufferList    m_vertexs;
//    IndexBuffer*        m_indices;
//    unsigned            m_indexStart;
//    unsigned            m_indexCount;
//    unsigned            m_vertexStart;
//    unsigned            m_vertexCount;
//    float               m_lodDistance;
//};

// like model submesh
class CUTE_CORE_API Mesh
{
public:
    struct Submesh
    {
        
    };
    
private:
    typedef Vector<Geometry*> MeshList;
    AABox       m_aabb;
    VertexLayout m_vertexs;
    IndexBuffer*    m_index;
    MeshList    m_meshes;
    Material*   m_materials;
};

//class CUTE_CORE_API MeshBuilder
//{
//    
//};
//
//class CUTE_CORE_API MeshFilter
//{
//public:
//    
//private:
//    Mesh m_mesh;
//};
//
//// 需要提供：Materail，和Mesh
//class CUTE_CORE_API Renderable
//{
//public:
//    virtual Mesh*       getMesh(float distance);
//    virtual Geometry*   getGeometry(float distance);
//    virtual Materail*   getMaterial();
//    
//private:
//    
//}
//
//class CUTE_CORE_API MeshRender : public Renderable
//{
//public:
//};
//
//class CUTE_CORE_API AnimatedRender : public Renderable
//{
//    
//};
//
//class CUTE_CORE_API SkinnedRender
//{
//public:
//    void update()
//    {
//        m_material.setMatrix4();
//    }
//    
//    Transform* getBones();
//    
//private:
//    //Animation m_nimation;
//    Material m_material;
//    Array<Matrix4> m_poses;
//    //Matrix4*  m_poses;
//};

CUTE_NS_END
