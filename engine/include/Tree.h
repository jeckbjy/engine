#pragma once
#include "Platform.h"
#include <cassert>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <set>
#include <queue>
#include <algorithm>
#include <cstddef>

CU_NS_BEGIN

// Ҫ������ǵ�����
template<typename T>
class Tree;

// ˫��ѭ������,���Խ�ʡһ��ָ��
template<class T>
class TreeNode
{
	friend class Tree<T>;
public:
	TreeNode() :m_parent(NULL), m_prev_sibling(NULL), m_next_sibling(NULL), m_first_child(NULL), m_last_child(NULL){}
	~TreeNode()
	{
		// �ͷ��ӽڵ�,delete??
		T* node = m_first_child;
		while (node)
		{
			T* tmp = node;
			node = node->m_next_sibling;
			delete tmp;
		}
		m_first_child = NULL;
		m_last_child = NULL;
	}

	void setParent(T* node)
	{
		//assert(node != NULL);
		//if (m_parent == node)
		//	return;
		//if (m_parent)
		//	m_parent->removeChild(this);
		//m_parent = node;

	}
	void addChild(T* node)
	{
		//if (node->m_parent == this)
		//	return;
		//if (m_parent)
		//	m_parent->removeChild(this);
		//m_parent = NULL;
	}
	void removeChild(T* node)
	{
		//if (node->m_parent == this)
		//	return;
	}
	void addSibling(T* node){}
	void removeSibling(T* node){}
	size_t size() const
	{
		size_t ret = 0;
		T* node = m_first_child;
		while (node != NULL)
		{
			node = node->m_next_sibling;
			++ret;
		}
		return ret;
	}
protected:
	T* m_parent;
	// sibling for list node
	T* m_prev_sibling;
	T* m_next_sibling;
	// child list
	T* m_first_child;
	T* m_last_child;
};

// ��ȡ��Ϣ
template<typename T>
struct tree_traits
{

};

template<typename T>
class Tree
{
	typedef T tree_node;
	//typedef TreeNode<T> tree_node;
public:
	template<typename Itor>
	class iterator_base
	{
	public:
		iterator_base(tree_node* node) :m_node(node){}
		T& operator *() const { return *m_node; }
		T* operator->() const { return m_node; }

		bool operator!()const { return m_node == 0; }
		operator bool() const { return m_node != 0; }

		bool operator==(const Itor& rhs) const { return m_node == rhs.m_node; }
		bool operator!=(const Itor& rhs) const { return m_node != rhs.m_node; }

		friend Itor operator++(Itor& x, int)
		{
			Itor r(x);
			++x;
			return r;
		}
		friend Itor& operator+=(Itor& lhs, uint num)
		{
			while (num-- > 0)
				++lhs;
			return lhs;
		}

	protected:
		void move_to_leaf()
		{
			assert(m_node != NULL);
			while (m_node->m_first_child)
				m_node = m_node->m_first_child;
		}
	protected:
		tree_node* m_node;
	};
	// ����root
	template<typename Itor>
	class iterator_root : public iterator_base<Itor>
	{
	public:
		iterator_root(tree_node* node) :iterator_base<Itor>(node), m_root(node){}
	protected:
		tree_node* m_root;
	};

	template<typename T>
	class iterator_backward
	{
		friend T operator--(T& x, int)
		{
			T r(x);
			--x;
			return r;
		}
		friend T& operator-=(T& lhs, size_t num)
		{
			while (num-- > 0)
				--lhs;
			return lhs;
		}
	};
	// ������ȱ���,ע��skipû��Ч��,ʹ��queue�ǿ��Ե�
	class breadth_iterator : public iterator_root<breadth_iterator>
	{
	public:
		breadth_iterator(tree_node* node = NULL) :iterator_root(node), m_next_first(NULL){}
		breadth_iterator& operator++();
	private:
		tree_node* m_next_first;	// ��һ���ո��ڵ�
	};
	// �����������,first node, then children
	class pre_order_iterator : public iterator_root<pre_order_iterator>
	{
		bool m_skip_current_children;	// ���Ե�ǰ�ڵ㣬�ⲿ����ǿ��ʹ��
	public:
		pre_order_iterator(tree_node* node = NULL) :iterator_root(node), m_skip_current_children(false){}
		pre_order_iterator& operator++();
		void skip_children(bool flag = true) { m_skip_current_children = flag; }
	};
	// �����������,first child, then node
	class post_order_iterator : public iterator_root<post_order_iterator>
	{
		bool m_skip_current_children;	// ���Ե�ǰ�ڵ㣬�ⲿ����ǿ��ʹ��
	public:
		post_order_iterator() :iterator_root(NULL), m_skip_current_children(false){}
		post_order_iterator(tree_node* node) :iterator_root(node), m_skip_current_children(false){ move_to_leaf(); }
		post_order_iterator& operator++();
		void skip_children(bool flag = true) { m_skip_current_children = flag; }
	};

	// Ҷ�ӽڵ�, post order
	class leaf_iterator : public iterator_root<leaf_iterator>
	{
	public:
		leaf_iterator() :iterator_root(NULL){}
		leaf_iterator(tree_node* node) :iterator_root(node){ move_to_leaf(); }
		leaf_iterator& operator++();
	};
	// �����ӽڵ�����ֵܽڵ�,����Ҫroot�ڵ�
	class sibling_iterator : public iterator_base<sibling_iterator>, public iterator_backward<sibling_iterator>
	{
	public:
		sibling_iterator(tree_node* node = NULL) :iterator_base(node){}
		sibling_iterator& operator++();
		sibling_iterator& operator--();
	};
	typedef sibling_iterator children_iterator;
public:
	static sibling_iterator	 begin_sibling(tree_node* node) { return sibling_iterator(node); }
	static sibling_iterator	 end_sibling() { return sibling_iterator(); }
	static children_iterator begin_children(tree_node* node) { return children_iterator(node->m_first_child); }
	static children_iterator end_children() { return children_iterator(); }

public:
	Tree() :m_root(NULL){}
	Tree(tree_node* root) :m_root(root){}
	~Tree() { CU_DELETE(m_root); }

	pre_order_iterator  begin() const { return pre_order_iterator(m_root); }
	pre_order_iterator  end() const { return pre_order_iterator(); }
	post_order_iterator begin_post() const { return post_order_iterator(m_root); }
	post_order_iterator end_post() const{ return post_order_iterator(); }
	breadth_iterator	begin_breadth() const { return breadth_iterator(m_root); }
	breadth_iterator	end_breadth() const { return breadth_iterator(); }
	leaf_iterator		begin_leaf() const { return leaf_iterator(m_root); }
	leaf_iterator		end_leaf() const { return leaf_iterator(); }
	sibling_iterator	begin_sibling() const { return sibling_iterator(m_root); }
	children_iterator	begin_children() const { return begin_children(m_root); }

	bool operator!()const { return m_root == 0; }
	operator bool() const { return m_root != 0; }
protected:
	tree_node* m_root;
};

//////////////////////////////////////////////////////////////////////////
// breadth_iterator
//////////////////////////////////////////////////////////////////////////
template<typename T>
typename Tree<T>::breadth_iterator& Tree<T>::breadth_iterator::operator++()
{
	assert(m_node != NULL);
	if (m_node == NULL)
		return *this;
	if (m_node == m_root)
	{// ��ʼ
		m_node = m_root->m_first_child;
		m_next_first = NULL;
	}
	else if (m_node->m_next_sibling == NULL)
	{// ����ͬ����һ���ڵ�
		tree_node* parent = m_node->m_parent;
		if (parent != m_root)
		{
			while (parent && parent->m_first_child == NULL)
				parent = parent->m_next_sibling;
		}
		if (parent != m_root && parent != NULL)
		{
			m_node = parent->m_first_child;
		}
		else
		{
			m_node = m_next_first;
			m_next_first = NULL;
		}
	}
	// ͬʱ������һ���׽ڵ�
	if (m_next_first == NULL && m_node && m_node->m_first_child != NULL)
		m_next_first = m_node->m_first_child;

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// ������ȱ���,������� pre_order_iterator
//////////////////////////////////////////////////////////////////////////
template<typename T>
typename Tree<T>::pre_order_iterator& Tree<T>::pre_order_iterator::operator++()
{
	assert(m_node != NULL);
	if (m_node == NULL)
		return *this;

	if (!m_skip_current_children && m_node->m_first_child != NULL)
	{
		m_node = m_node->m_first_child;
	}
	else
	{// ���ϻ���,�����������б�β
		m_skip_current_children = false;
		while (!m_node->m_next_sibling)
		{
			m_node = m_node->m_parent;
			if (m_node == NULL || m_node == m_root)
			{// ��������
				m_node = NULL;
				return *this;
			}
		}
		m_node = m_node->m_next_sibling;
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// ������ȱ���,�������
//////////////////////////////////////////////////////////////////////////
template<typename T>
typename Tree<T>::post_order_iterator& Tree<T>::post_order_iterator::operator++()
{
	assert(m_node != NULL);
	if (m_node == NULL)
		return *this;

	if (m_node == m_root)
	{// �������
		m_node = NULL;
		return *this;
	}
	if (m_node->m_next_sibling == 0)
	{
		m_node = m_node->m_parent;
		m_skip_current_children = false;
	}
	else
	{// ����
		m_node = m_node->m_next_sibling;
		if (m_skip_current_children){
			m_skip_current_children = false;
		}
		else{
			move_to_leaf();
		}
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// use post_order but just leaf
//////////////////////////////////////////////////////////////////////////
template<typename T>
typename Tree<T>::leaf_iterator& Tree<T>::leaf_iterator::operator++()
{
	assert(m_node != 0);
	if (m_node == NULL)
		return *this;
	if (m_node == m_root)
	{
		m_node = NULL;
		return *this;
	}
	if (m_node->m_first_child != 0)
	{// current node is no longer leaf (children got added)
		move_to_leaf();
	}
	else
	{
		while (m_node->m_next_sibling == 0)
		{// û���ӽڵ��ˣ����ϻ���
			m_node = m_node->m_parent;
			if (m_node == m_root)
			{
				m_node = NULL;
				return *this;
			}
		}
		m_node = m_node->m_next_sibling;
		move_to_leaf();
	}
	return *this;
}
//////////////////////////////////////////////////////////////////////////
// sibling
//////////////////////////////////////////////////////////////////////////
template<typename T>
typename Tree<T>::sibling_iterator& Tree<T>::sibling_iterator::operator++()
{
	if (m_node)
		m_node = m_node->m_next_sibling;
	return *this;
}

template<typename T>
typename Tree<T>::sibling_iterator& Tree<T>::sibling_iterator::operator--()
{
	if (m_node)
		m_node = m_node->m_prev_sibling;
	return *this;
}

CU_NS_END