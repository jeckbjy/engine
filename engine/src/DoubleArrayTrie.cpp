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

	// ��ʼ��
	for (size_t i = m_size; i < capacity; ++i)
		memset(&(new_array[i]), 0, sizeof(unit_t));

	// ��ֵ
	delete m_array;
	m_array = new_array;
	m_size = capacity;
}

void DoubleArrayTrie::build(size_t num_keys, const key_t** keys)
{
	clear();

	// Ҫ�����������
	if (num_keys == 0)
		return;

	resize(num_keys * 10);
	m_array[0].base = 1;
	size_t next_pos = 1;	// ���ڱ�ʶ��һ�μ����ʼλ��
	size_t unit_num = 1;	// ��¼�����ַ�����

	// 
	std::vector<DATNode*> pools;
	std::vector<DATNode*> siblings;
	std::queue<DATNode*>  nodes;

	DATNode* root = new DATNode();
	root->set(0, 0, 0, num_keys);
	nodes.push(root);

	// �����������
	while (nodes.empty())
	{
		DATNode* parent = nodes.front();
		nodes.pop();

		siblings.clear();

		// fetch
		key_t prev_code = 0;
		key_t curr_code;
		for (int i = parent->left; i < parent->right; ++i)
		{
			const key_t* str = keys[i];

			curr_code = str[parent->depth];

			if (prev_code > curr_code)
			{// ��������,�ⲿӦ�ñ�֤����
				return;
			}

			if (curr_code != prev_code || siblings.empty())
			{
				DATNode* tmp_node = new DATNode();
				tmp_node->set(curr_code, parent->depth + 1, i, 0);

				if (!siblings.empty())
					siblings[siblings.size() - 1]->right = i;

				siblings.push_back(tmp_node);
			}

			prev_code = curr_code;
		}

		if (!siblings.empty())
			siblings[siblings.size() - 1]->right = parent->right;

		if (siblings.empty())
			continue;

		// insert
		// ���ҿ����ַ�
		int base_pos = next_pos - siblings[0]->code;
		int back_code = siblings[siblings.size() - 1]->code;
		// ����һ�����Բ����λ��
		for (;;++base_pos)
		{
			if (m_array[base_pos].label != 0)
			{
				continue;
			}

			// У���Ƿ�Խ��
			if (base_pos + back_code >= m_size)
			{
				size_t new_size = 2 * m_size;
				resize(new_size);
			}

			for (size_t i = 0; i < siblings.size(); ++i)
			{
				size_t hash_index = base_pos + siblings[i]->code;
				if (m_array[hash_index].label != 0)
				{
					break;
				}
			}
		}

		// �ҵ��ˣ�����label��check
		for (size_t i = 0; i < siblings.size(); ++i)
		{
			DATNode* node = siblings[i];
			node->index = base_pos + node->code;
			// ����check
			m_array[node->index].label = node->code;
			nodes.push(node);
		}

		// ����baseֵ
		m_array[parent->index].base = base_pos;
		m_array[parent->index].leaf = 1;
	}
}

int DoubleArrayTrie::match(MatchMode mode, const key_t* key, size_t length /* = 0 */, size_t* node_pos /* = 0 */) const
{
	// 
	// ��У����Դ�Ƿ���root
	if (node_pos == 0)
	{

	}
	// ����ֵ:0:��ʾû��ƥ�䵽������0:��ʾƥ�䵽�ĳ���
	size_t curr_index = node_pos ? *node_pos : 0;
	if (curr_index >= m_size)
		return -1;

	// û������
	if (curr_index >= m_size)
		return -1;

	if (length == 0)
		length = std::char_traits<key_t>::length(key);

	// ��ʼ����
	size_t last_index = 0;
	size_t last_match = 0;//��һ��ƥ�䵽�ĸ���
	int32  next_index;
	key_t ch;
	unit_t* unit;

	for (size_t i = 0; i < length; ++i)
	{
		ch = key[i];
		next_index = m_array[curr_index].base + ch;
		assert(next_index > 0);
		if (next_index >= m_size)
			break;

		unit = &(m_array[next_index]);
		// ��һ�£�˵��û��ƥ����
		if (unit->label != ch)
			break;

		// ƥ��ɹ�
		// �ж��Ƿ��Ǵ���
		if (unit->leaf != 0)
		{
			last_index = next_index;
			last_match = i + 1;
			if (mode == MATCH_PREFIX_MIN)
				break;
		}

		// ��һ��
		curr_index = next_index;
	}

	if (node_pos != 0)
		*node_pos = last_index;

	// ���ؽ��
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
