#pragma once
#include "API.h"

CU_NS_BEGIN
//////////////////////////////////////////////////////////////////////////
// node
//////////////////////////////////////////////////////////////////////////
// pre define
template<class T, int N>
struct ListTraits;

// 定义节点
template<class T, int N = 1>
class ListNode{};

template<class T>
class ListNode<T, 1>
{
	friend struct ListTraits<T, 1>;
protected:
	T* m_prev1;
	T* m_next1;
public:
	ListNode() :m_prev1(0), m_next1(0){}
};

template<class T>
class ListNode<T, 2>
{
	friend struct ListTraits<T, 2>;
protected:
	T* m_prev2;
	T* m_next2;
public:
	ListNode() :m_prev2(0), m_next2(0){}
};

template<class T>
class ListNode2
{
	friend struct ListTraits<T, 1>;
	friend struct ListTraits<T, 2>;
protected:
	T* m_prev1;
	T* m_next1;
	T* m_prev2;
	T* m_next2;
public:
	ListNode2() :m_prev1(0), m_next1(0), m_prev2(0), m_next2(0){}
};

//////////////////////////////////////////////////////////////////////////
// 萃取
//////////////////////////////////////////////////////////////////////////
template<typename T, int N>
struct ListTraits
{
	typedef T node_t;
	inline static node_t* get_prev(node_t* n) { return (node_t*)n->m_prev; }
	inline static node_t* get_next(node_t* n) { return (node_t*)n->m_next; }

	inline static void set_prev(node_t* n1, node_t* n2) { n1->m_prev = n2; }
	inline static void set_next(node_t* n1, node_t* n2) { n1->m_next = n2; }
};

template<typename T>
struct ListTraits<T, 1>
{
	typedef T node_t;
	inline static node_t* get_prev(node_t* n) { return (node_t*)n->m_prev1; }
	inline static node_t* get_next(node_t* n) { return (node_t*)n->m_next1; }

	inline static void set_prev(node_t* n1, node_t* n2) { n1->m_prev1 = n2; }
	inline static void set_next(node_t* n1, node_t* n2) { n1->m_next1 = n2; }
};

template<typename T>
struct ListTraits<T, 2>
{
	typedef T node_t;
	inline static node_t* get_prev(node_t* n) { return (node_t*)n->m_prev2; }
	inline static node_t* get_next(node_t* n) { return (node_t*)n->m_next2; }

	inline static void set_prev(node_t* n1, node_t* n2) { n1->m_prev2 = n2; }
	inline static void set_next(node_t* n1, node_t* n2) { n1->m_next2 = n2; }
};

template<typename T, bool isConst>
struct list_iterator_base
{

};

template<typename T>
struct list_iterator_base<T, true>
{

};

template<typename T, bool isConst>
struct list_iterator : public list_iterator_base<T, isConst>
{
	list_iterator& operator++(){ return *this; }
	list_iterator& operator--(){ return *this; }
	list_iterator& operator+=(size_t offset) { return *this; }
};

//////////////////////////////////////////////////////////////////////////
// List:双向循环链表,所有node必须由list操作
// todo:size traits, iterator traits
//////////////////////////////////////////////////////////////////////////
template<typename T, int N = 1>
class List
{
protected:
	typedef ListTraits<T, N> traits;
	typedef T node_t;
	inline static node_t* get_prev(node_t* n) { return traits::get_prev(n); }
	inline static node_t* get_next(node_t* n) { return traits::get_next(n); }
	inline static void set_prev(node_t* n1, node_t* n2) { traits::set_prev(n1, n2); }
	inline static void set_next(node_t* n1, node_t* n2) { traits::set_next(n1, n2); }
	inline static void set_next_prev(node_t* n1, node_t* n2) { set_prev(get_next(n1), n2); }	// n1->m_next->m_prev = n2;
	inline static void set_prev_next(node_t* n1, node_t* n2) { set_next(get_prev(n1), n2); }	// n1->m_prev->m_next = n2;
	inline static void set_node_next(node_t* n1, node_t* n2) { set_next(n1, get_next(n2)); }	// n1->m_next = n2->m_next;
	inline static void destroy_node(node_t* node) { set_prev(node, NULL); set_next(node, NULL); }
public:
	class iterator
	{
		node_t *m_head, *m_curr;
	public:
		iterator(node_t* node = NULL) :m_head(node), m_curr(node){}
		iterator& operator++()
		{
			m_curr = get_next(m_curr) != m_head ? get_next(m_curr) : NULL;
			return *this;
		}
		iterator& operator--()
		{
			m_curr = get_prev(m_curr);
			return (*this);
		}
		iterator operator++(int){ iterator itor(*this); ++(*this); return itor; }
		iterator operator--(int){ iterator itor(*this); --(*this); return itor; }
		node_t*  operator*() { return m_curr;}
		node_t*	 operator->(){ return m_curr; }

		bool operator!()const { return m_curr == 0; }
		operator bool() const { return m_curr != 0; }
		operator node_t*() const { return m_curr; }
		bool operator==(const iterator& rhs) const { return m_curr == rhs.m_curr; }
		bool operator!=(const iterator& rhs) const { return m_curr != rhs.m_curr; }
	};

	
public:
	List() :m_head(NULL){}
	~List() { clear(); }
	void append(node_t* data);		// 从尾部插入
	void prepend(node_t* data);		// 从头部插入
	
	void insert(node_t* pos, node_t* data);
	void erase(node_t* pos);
	
	void clear();
	void release();
	void free();

	void swap(List& rhs);
	size_t size() const;
	void push_back(node_t* data) { append(data); }
	void push(node_t* data) { append(data); }
	bool empty() const { return m_head == 0; }

	void pop_front() { erase(begin()); }
	node_t* front() const { return m_head; }

	iterator begin() const { return iterator(m_head); }
	iterator end() const { return iterator(); }
	iterator& erase(iterator& itor) { erase(*(itor++)); return itor; }
private:
	node_t* m_head;
};

template<class T, int N>
void List<T, N>::swap(List& rhs)
{
	std::swap(m_head, rhs.m_head);
}

template<class T, int N>
size_t List<T, N>::size() const
{
	if (!m_head)
		return 0;
	size_t result = 1;
	node_t* node = m_head;
	node_t* tail = get_prev(m_head);
	while (node != tail)
		++result;
	return result;
}

template<typename T, int N>
void List<T, N>::append(node_t* node)
{
	if (m_head == NULL)
	{
		m_head = node;
		set_next(node, node);
		set_prev(node, node);
	}
	else
	{
		node_t* tail = get_prev(m_head);
		set_prev(node, tail);
		set_next(node, m_head);
		set_next(tail, node);
		set_prev(m_head, node);
	}
}

template<typename T, int N>
void List<T, N>::prepend(node_t* node)
{
	if (m_head == NULL)
	{
		set_next(node, node);
		set_prev(node, node);
	}
	else
	{
		set_next(node, m_head);
		set_prev(node, get_prev(m_head));
		set_prev(m_head, node);
	}
	m_head = node;
}

template<typename T, int N>
void List<T, N>::insert(node_t* prev_node, node_t* next_node)
{
	set_next_prev(prev_node, next_node);
	set_node_next(next_node, prev_node);
	set_next(prev_node, next_node);
	set_next(next_node, prev_node);
}

template<typename T, int N>
void List<T, N>::erase(node_t* node)
{
	// 校验，说明已经取消关联
	if (get_next(node) == NULL)
		return;
	if (m_head == node)
	{
		m_head = NULL;
	}
	else
	{
		set_prev_next(node, get_next(node));
		set_next_prev(node, get_prev(node));
		if (m_head == node)
			m_head = get_next(node);
	}

	destroy_node(node);
}

template<typename T, int N>
void List<T, N>::clear()
{
	node_t* node = m_head;
	node_t* temp;
	while (node)
	{
		temp = node;
		node = get_next(node);
		destroy_node(temp);
	}
	m_head = 0;
}

template<typename T, int N>
void List<T, N>::release()
{
	node_t* node = m_head;
	node_t* temp;
	while (node)
	{
		temp = node;
		node = get_next(node);
		destroy_node(temp);
		temp->release();
	}
	m_head = 0;
}

template<typename T, int N>
void List<T, N>::free()
{
	node_t* node = m_head;
	node_t* temp;
	while (node)
	{
		temp = node;
		node = get_next(node);
		destroy_node(temp);
		delete temp;
	}
	m_head = 0;
}

CU_NS_END