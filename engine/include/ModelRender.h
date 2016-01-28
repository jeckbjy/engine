#pragma once
#include "Model.h"
#include "Component.h"
#include "Transform.h"

CU_NS_BEGIN

// ��̬��Ƥ��Ϣ,Avataʱ���Թ���
class CU_API MeshSkin : public Ref
{
public:
	MeshSkin(Skeleton* skeleton);
	~MeshSkin();

	void reset();
	void update(Transform* root, uint32 now);	// ���¹Ǽ�λ��

private:
	Skeleton* m_skeleton;
	Matrix4*  m_matrixs;
	uint32	  m_timestamp;	// �ϴθ���ʱ���
};
typedef SharedPtr<MeshSkin> MeshSkinPtr;

// ��Ⱦģ�� 
class CU_API ModelRender : public Component
{
public:
	ModelRender(Model* model);
	~ModelRender();

	void draw();

	void attach(MeshSkin* skin) { m_skin = skin; }
	void detach() { m_skin.reset(); }

	MeshSkin* getSkin() { return m_skin.get(); }

protected:
	Model* m_model;
	MeshSkinPtr m_skin;
};

CU_NS_END
