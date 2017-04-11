//! Algorithm
#include "Cute/DoubleArrayTrie.h"

CUTE_NS_BEGIN

struct DATNode
{
	int code;
	int depth;
	int left;
	int right;
	int index;

	void set(int code, int depth, int left, int right)
	{
		this->code = code;
		this->depth = depth;
		this->left = left;
		this->right = right;
	}
};

DoubleArrayTrie::DoubleArrayTrie()
	: m_size(0)
	, m_array(NULL)
{
}

DoubleArrayTrie::~DoubleArrayTrie()
{
}

bool DoubleArrayTrie::open()
{
	return true;
}

void DoubleArrayTrie::save()
{

}

void DoubleArrayTrie::clear()
{
	if (m_array != NULL)
	{
		delete m_array;
		m_array = 0;
		m_size = 0;
	}
}

void DoubleArrayTrie::resize(size_t capacity)
{
	if (capacity <= m_size)
		return;

	unit_t* new_array = new unit_t[capacity];
	if (m_size > 0)
	{
		for (size_t i = 0; i < m_size; ++i)
			new_array[i] = m_array[i];
	}

	// 初始化
	for (size_t i = m_size; i < capacity; ++i)
		memset(&(new_array[i]), 0, sizeof(unit_t));

	// 赋值
	delete m_array;
	m_array = new_array;
	m_size = capacity;
}

//void DoubleArrayTrie::build(std::vector<String>& words, int flags)
//{
//
//}

void DoubleArrayTrie::build(size_t num_keys, const key_t** keys, int flags)
{
	struct SortCmp
	{
		int depth;
		SortCmp(int depth) :depth(depth){}

		bool operator () (const key_t* lhs, const key_t* rhs) const
		{
			return (uchar)(lhs[depth]) < (uchar)(rhs[depth]);
		}
	};

	clear();

	// 要求必须是有序
	if (num_keys == 0)
		return;

	bool needSort = (flags & FLAG_SORTED) == 0;
	bool isDynamic = (flags & FLAG_DYNAMIC) != 0;
	bool isRoot1 = isDynamic || ((flags & FLAG_ROOT1) != 0);

	if (needSort)
		std::sort(keys, keys + num_keys, SortCmp(0));

	// 用于标识下一次检测起始位置
	size_t next_pos;
	if (isDynamic)
		next_pos = 256;
	else if (isRoot1)
		next_pos = keys[0][0] + 1;
	else
		next_pos = 1;

	resize(num_keys * 10);
	m_array[0].base = 1;

	std::queue<DATNode*>  nodes;
	std::vector<DATNode*> siblings;
	std::vector<DATNode*> pools;

	DATNode* root = new DATNode();
	root->set(0, 0, 0, num_keys);
	nodes.push(root);

	int left, right, depth, index, code;

	// 广度优先搜索
	while (!nodes.empty())
	{
		siblings.clear();
		DATNode* parent = nodes.front();
		nodes.pop();
		pools.push_back(parent);

		left  = parent->left;
		right = parent->right;
		depth = parent->depth;
		index = parent->index;
		code  = parent->code;

		// 首先排序子节点
		if (needSort)
			std::sort(keys + left, keys + right, SortCmp(depth));

		// fetch
		uchar prev_code = 0;
		uchar curr_code;
		for (int i = left; i < right; ++i)
		{
			const key_t* str = keys[i];
			// 偏移1，因为有0需要单独处理
			curr_code = (uchar)str[depth];

			if (prev_code > curr_code)
			{// 发生错误,外部应该保证有序
				return;
			}

			if (curr_code != prev_code || siblings.empty())
			{
				DATNode* temp;
				if (!pools.empty())
				{
					temp = pools.back();
					pools.pop_back();
				}
				else
				{
					temp = new DATNode();
				}

				temp->set(curr_code, depth + 1, i, 0);

				if (!siblings.empty())
					siblings[siblings.size() - 1]->right = i;

				siblings.push_back(temp);
			}

			prev_code = curr_code;
		}

		if (!siblings.empty())
			siblings[siblings.size() - 1]->right = right;

		if (siblings.empty())
			continue;

		// insert
		// 查找可用字符
		// 先更新一下next_pos
		for (size_t i = next_pos; i < m_size; ++i)
		{
			if (m_array[i].check == 0)
			{
				next_pos = i;
				break;
			}
		}

		int base_pos = next_pos - siblings[0]->code;
		int back_code = siblings[siblings.size() - 1]->code;
		// 查找一个可以插入的位置
		for (;;++base_pos)
		{
			//if (m_array[base_pos].check != 0)
			//	continue;

			// 校验是否越界
			size_t epos = size_t(base_pos + back_code);
			if (epos >= m_size)
			{
				size_t new_size = 2 * std::max(epos, m_size);
				resize(new_size);
			}

			bool isAll = true;
			for (size_t i = 0; i < siblings.size(); ++i)
			{
				size_t hash_index = base_pos + siblings[i]->code;
				if (m_array[hash_index].check != 0)
				{
					isAll = false;
					break;
				}
			}

			if (isAll)
				break;
		}

		bool isWord = false;
		// 找到了，设置label即check
		for (size_t i = 0; i < siblings.size(); ++i)
		{
			DATNode* node = siblings[i];
			int32 next_index = base_pos + node->code;
			node->index = next_index;

			if (node->code != 0)
			{
				m_array[next_index].check = base_pos;
				nodes.push(node);
			}
			else
			{// 标识结束
				isWord = true;
				pools.push_back(node);
			}
		}

		// 设置base值
		m_array[index].base = base_pos;
		if (isWord)
			m_array[index].word = 1;
	}

	// 释放空间
	for (size_t i = 0; i < pools.size(); ++i)
	{
		delete pools[i];
	}

	pools.clear();
}

int DoubleArrayTrie::match(MatchMode mode, const key_t* key, size_t length /* = 0 */, size_t* node_pos /* = 0 */) const
{
	// 返回值:0:表示没有匹配到，大于0:表示匹配到的长度
	size_t curr_index = node_pos ? *node_pos : 0;
	if (curr_index >= m_size)
		return -1;

	// 没有数据
	if (curr_index >= m_size)
		return -1;

	if (length == 0)
		length = std::char_traits<key_t>::length(key);

	// 开始计算
	size_t last_index = 0;
	size_t last_match = 0;//上一次匹配到的个数
	int32  next_index;
	uchar ch;
	unit_t* unit;

	for (size_t i = 0; i < length; ++i)
	{
		ch = key[i];
		next_index = m_array[curr_index].base + ch;
		if (next_index <= 0 || next_index >= m_size)
			break;

		unit = &(m_array[next_index]);
		// 不一致，说明没有匹配上
		if (unit->check != m_array[curr_index].base)
			break;

		// 匹配成功
		// 判断是否是词组
		if (unit->word != 0)
		{
			last_index = next_index;
			last_match = i + 1;
			if (mode == MATCH_PREFIX_MIN)
				break;
		}

		// 下一轮
		curr_index = next_index;
	}

	if (node_pos != 0)
		*node_pos = last_index;

	// 返回结果
	if (mode == MATCH_STRICT)
	{
		return last_match == length ? last_match : 0;
	}
	else
	{
		return last_match;
	}
}

CUTE_NS_END
