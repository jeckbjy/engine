#pragma once
#include "Graphics.h"

CU_NS_BEGIN

// 一个渲染批次
struct CU_API Batch
{
	uint64		sortKey;
	uint8		renderOrder;	// 8-bit render order modifier from material.
	uint8		lightMask;		// 8-bit light mask for stencil marking in deferred rendering.
	float		distance;		// Distance from camera.
	Geometry*	geometry;
	Material*	material;

	Batch();
	void prepare();
	void draw();

	bool isTransparent() { return false; }
};

// 渲染批次队列
class CU_API BatchQueue
{
	typedef Vector<Batch*> BatchVec;

public:
	void draw();

	void push(Batch* batch);
	void sortFrontToBack();
	void sortBackToFront();
	void clear();

private:
	BatchVec m_batches;
};

CU_NS_END
