#pragma once
#include "Model.h"
#include "Component.h"
#include "Transform.h"
#include "Drawable.h"

CU_NS_BEGIN

// 动态蒙皮信息,Avata时可以共享
class CU_API MeshSkin : public Ref
{
public:
	MeshSkin(Skeleton* skeleton);
	~MeshSkin();

	void reset();
	void update(Transform* root, uint32 now);	// 更新骨架位置

private:
	Skeleton* m_skeleton;
	Matrix4*  m_matrixs;
	uint32	  m_timestamp;	// 上次更新时间戳
};
typedef SharedPtr<MeshSkin> MeshSkinPtr;

// 渲染模型 
class CU_API ModelRender : public Drawable
{
public:
	ModelRender(Model* model);
	~ModelRender();

	void draw(View* view);

	void attach(MeshSkin* skin) { m_skin = skin; }
	void detach() { m_skin.reset(); }

	MeshSkin* getSkin() { return m_skin.get(); }

protected:
	typedef std::vector<MaterialPtr> MaterialVec;
	Model* m_model;
	MaterialVec m_materials;	// 动态的材质，如果没有设置，则使用默认的Model中材质
	MeshSkinPtr m_skin;
};

CU_NS_END

