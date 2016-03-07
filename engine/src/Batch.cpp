#include "Batch.h"

CU_NS_BEGIN

inline bool CmpBackToFront(Batch* lhs, Batch* rhs)
{
	if (lhs->renderOrder != rhs->renderOrder)
		return lhs->renderOrder < rhs->renderOrder;
	else if (lhs->distance != rhs->distance)
		return lhs->distance > rhs->distance;
	else
		return lhs->sortKey < rhs->sortKey;
}

inline bool CmpFrontToBack(Batch* lhs, Batch* rhs)
{
	if (lhs->renderOrder != rhs->renderOrder)
		return lhs->renderOrder < rhs->renderOrder;
	else if (lhs->distance != rhs->distance)
		return lhs->distance < rhs->distance;
	else
		return lhs->sortKey < rhs->sortKey;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
Batch::Batch()
{

}

void Batch::prepare()
{

}

void Batch::draw()
{

}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void BatchQueue::draw()
{

}

void BatchQueue::push(Batch* batch)
{
	m_batches.push_back(batch);
}

void BatchQueue::sortBackToFront()
{
	std::sort(m_batches.begin(), m_batches.end(), &CmpBackToFront);
}

void BatchQueue::sortFrontToBack()
{
	std::sort(m_batches.begin(), m_batches.end(), &CmpFrontToBack);
}

void BatchQueue::clear()
{
	m_batches.clear();
}

CU_NS_END
