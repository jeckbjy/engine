#pragma once
#include "D3D11_API.h"

CUTE_NS_BEGIN

template<typename TState, typename TDesc>
class D3D11_StateMap
{
public:
	~D3D11_StateMap()
	{
		for (size_t i = 0; i < m_datas.size(); ++i)
			m_datas[i]->release();
		m_datas.clear();
	}

	TState* obtain(ID3D11DeviceN* device, const TDesc& desc)
	{
		uint32_t hashCode = desc.getHashCode();
		//
		int lastIndex;
		TState* state = find(desc, hashCode, lastIndex);
		if (state == NULL)
		{//
			state = new TState(device, desc, hashCode);
			if (lastIndex != -1)
				m_datas.insert(m_datas.begin() + lastIndex, state);
			else
				push(state);
		}

		state->retain();
		return state;
	}

	void clear(int threshold)
	{
		if (m_datas.size() < threshold)
			return;

		int lastIndex = -1;
		int len = (int)m_datas.size();
		for (int i = 0; i < len; ++i)
		{
			TState* state = m_datas[i];
			if (state->refs() > 1)
			{
				++lastIndex;
				if (lastIndex != i)
					m_datas[lastIndex] = i;
			}
			else
			{
				state->release();
				m_datas[i] = 0;
			}
		}

		int count = len - lastIndex;
		for (int i = 0; i < count; ++i)
		{
			m_datas.pop_back();
		}
	}

private:
	TState* find(const TDesc& desc, uint32_t hashCode, int& lastIndex)
	{
		lastIndex = -1;
		int index = findByHashCode(hashCode);
		if (index == -1)
			return NULL;

		// 先向后查找
		int len = (int)m_datas.size();
		TState* state;
		for (int i = index; i < len; ++i)
		{
			state = m_datas[i];
			if (state->getHashCode() != hashCode)
			{
				lastIndex = i - 1;
				break;
			}

			if (state->equal(desc))
			{
				return state;
			}
		}

		// 向前查找
		for (int i = index - 1; i >= 0; --i)
		{
			state = m_datas[i];
			if (state->getHashCode() != hashCode)
				break;

			if (state->equal(desc))
				return state;
		}

		return NULL;
	}

	int findByHashCode(uint32_t hashCode)
	{
		int len = (int)m_datas.size();
		if (len == 0)
			return -1;

		int low = 0;
		int high = len - 1;
		int mid;
		uint32_t key;
		while (low <= high)
		{
			mid = (low + high) >> 1;
			key = m_datas[mid]->getHashCode();

			if (key == hashCode)
				return mid;

			if (hashCode > key)
				low = mid + 1;
			else
				high = mid - 1;
		}

		return -1;
	}

	void push(TState* state)
	{
		// 二分插入,找到第一个比其小的
		int len = (int)m_datas.size();
		if (len == 0)
		{
			m_datas.push_back(state);
			return;
		}

		uint32_t hashCode = state->getHashCode();
		int low = 0;
		int high = len - 1;
		int mid;
		while (low <= high)
		{
			mid = (low + high) >> 1;
			if (hashCode >= m_datas[mid]->getHashCode())
				low = mid + 1;
			else
				high = mid - 1;
		}

		m_datas.insert(m_datas.begin() + low, state);
	}

private:
	typedef std::vector<TState*> DataVec;
	DataVec m_datas;
};

CUTE_NS_END
