#pragma once
#include "Model.h"
#include "Component.h"
#include "Transform.h"
#include "Drawable.h"

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
	MaterialVec m_materials;	// ��̬�Ĳ��ʣ����û�����ã���ʹ��Ĭ�ϵ�Model�в���
	MeshSkinPtr m_skin;
};

CU_NS_END

