//! Collection
#pragma once
#include "Cute/Foundation.h"
#include "Cute/Traits.h"
#include <utility>
#include <assert.h>

CUTE_NS_BEGIN

// todo:EASTL

class ListNode
{
public:
	ListNode() :m_prev(0), m_next(0){}

private:
	ListNode* m_prev;
	ListNode* m_next;

	template<class T, ListNode T::*, bool>
	friend class List;
    template<bool>
    friend class TSizeHolder;
};

template<bool hasSize>
struct TSizeHolder
{
    void inc(size_t count);
    void dec(size_t count);
    void reset();
    size_t get(ListNode* head) const;
};

template<>
struct TSizeHolder<true>
{
    TSizeHolder() :m_count(0){}
    inline void inc(size_t count) { m_count += count; }
    inline void dec(size_t count) { m_count -= count; }
    inline void reset() { m_count = 0; }
    inline size_t get(ListNode*) const { return m_count; }
    size_t m_count;
};

template<>
struct TSizeHolder<false>
{
    TSizeHolder() {}
    inline void inc(size_t) { }
    inline void dec(size_t) { }
    inline void reset(){}
    inline size_t get(ListNode* head) const
    {
        size_t count = 0;
        ListNode* node = head->m_next;
        while (node != head)
        {
            ++count;
            node = node->m_next;
        }
        
        return count;
    }
};

struct Tag1 {};
struct Tag2 {};

template<typename Tag = void>
class TListHook
{
public:
	ListNode hook;
};

typedef TListHook<void> ListHook;
typedef TListHook<Tag1>	ListHook1;
typedef TListHook<Tag2> ListHook2;

/*
// IntrusiveList
usage:
class MyNode : pulic ListHook, public ListHook1
{
public;
	ListNode m_node;
}

class ListOwner
{
	typedef List<MyNode> MyList1;
	typedef List<MyNode, &ListHook1::hook> MyList2;
	typedef List<MyNode, &MyNode::m_node>	 MyList3;
}
*/

// intrusive_list
template<typename T, ListNode T::* MemberPtr = &ListHook::hook, bool HasSize = true>
class List
{
public:
	class Iterator : public std::iterator<std::bidirectional_iterator_tag, T>
	{
	public:
		T* operator -> () 
		{
			return parent_from_member<T, ListNode>(m_node, MemberPtr); 
		}
		T* operator *  ()
		{
			return parent_from_member<T, ListNode>(m_node, MemberPtr);
		}

		bool operator==(const Iterator& rhs) const { return m_node == rhs.m_node; }
		bool operator!=(const Iterator& rhs) const { return m_node != rhs.m_node; }
		
		bool operator !() const 
		{
			return m_node == 0; 
		}

		operator bool() const 
		{
			return m_node != 0; 
		}

		Iterator& operator ++()
		{
			m_node = m_node->m_next;
			return *this;
		}

		Iterator& operator -- ()
		{
			m_node = m_node->m_prev;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator itor(*this);
			++(*this);
			return itor;
		}

		Iterator operator--(int)
		{
			Iterator itor(*this);
			--(*this);
			return itor;
		}

	private:
		Iterator(ListNode* node) 
			: m_node(node)
		{
		}

		ListNode* m_node;
		friend class List;
	};

	typedef std::reverse_iterator<Iterator> ReverseIterator;

public:
	List()
		: m_head(new_node())
	{
	}

	~List()
	{
		clear();
		delete m_head;
		m_head = 0;
	}

	void swap(List& other)
	{
		std::swap(m_head, other.m_head);
	}

	Iterator erase(Iterator position)
	{
		return erase(position.m_node);
	}

	Iterator insert(Iterator position, T* data)
	{
		return insert(position.m_node, data);
	}

	void push_front(T* data)
	{
		insert(begin(), data);
	}

	void push_back(T* data)
	{
		insert(end(), data);
	}

	void pop_front()
	{
		erase(begin());
	}

	void pop_back()
	{
		Iterator tmp = end();
		erase(--tmp);
	}

	void clear()
	{
		if (empty())
			return;

		m_size.reset();

		ListNode* node = m_head->m_next;
		ListNode* temp;

		// 断开连接,清空为0
		m_head->m_prev->m_next = NULL;

		while (node)
		{
			temp = node;
			node = node->m_next;
			temp->m_prev = NULL;
			temp->m_next = NULL;
		}

		// 重置head
		m_head->m_prev = m_head;
		m_head->m_next = m_head;
	}

	bool empty() const
	{
		return m_head->m_next == m_head;
	}

	size_t size() const
	{
		return m_size.get(m_head);
	}

	Iterator begin()
	{
		return Iterator(m_head->m_next);
	}

	Iterator end()
	{
		return Iterator(m_head);
	}

	ReverseIterator rbegin()
	{
		return ReverseIterator(end());
	}

	ReverseIterator rend()
	{
		return ReverseIterator(begin());
	}

	Iterator back()
	{
		return Iterator(m_head->m_prev);
	}

private:
	ListNode* erase(ListNode* node)
	{
		assert(node != m_head);
		assert(node->m_next != 0);

		ListNode* next = node->m_next;

		node->m_prev->m_next = node->m_next;
		node->m_next->m_prev = node->m_prev;
		node->m_next = 0;
		node->m_prev = 0;

		m_size.dec(1);
		return next;
	}

	ListNode* insert(ListNode* curr, T* data)
	{
		// 在节点的前边插入节点
		ListNode* node = get_node(data);
		assert(curr->m_next != 0);
		assert(node->m_prev == 0);	// 不能属于某一个链表

		node->m_next = curr;
		node->m_prev = curr->m_prev;
		curr->m_prev->m_next = node;
		curr->m_prev = node;

		m_size.inc(1);

		return node;
	}

	inline ListNode* get_tail()
	{
		return m_head->m_prev;
	}

	inline ListNode* get_node(T* ptr)
	{
		return &(ptr->*MemberPtr);
	}

	inline ListNode* new_node()
	{
		ListNode* node = new ListNode();
		node->m_next = node;
		node->m_prev = node;
		return node;
	}

private:
	typedef TSizeHolder<HasSize> SizeHolder;

	ListNode*	m_head;
	SizeHolder	m_size;
};

CUTE_NS_END
