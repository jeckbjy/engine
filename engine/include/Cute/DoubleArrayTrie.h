#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// todo:Aho Corasick��ģʽƥ��
// deterministic finite automaton ��DFA
// c:    https://linux.thai.net/~thep/datrie/datrie.html
// c++:	 https://github.com/s-yata/darts-clone
// java: https://github.com/komiya-atsushi/darts-java
// java: http://www.cnblogs.com/zhangchaoyang/articles/4508266.html
// ���֧�����ģ� tail
/*
array[0]:�洢��root��Ϣ��base:����Ϊ����,���������Ͳ���
todo:��δ���
*/
class CUTE_CORE_API DoubleArrayTrie
{
public:
	enum MatchMode
	{
		MATCH_STRICT,		// �ϸ�ƥ��
		MATCH_PREFIX_MIN,	// ��Сǰ׺ƥ��
		MATCH_PREFIX_MAX,	// ���ǰ׺ƥ��
	};

	typedef char key_t;
	typedef const char* str_t;

	DoubleArrayTrie();
	~DoubleArrayTrie();

	bool open();
	void save();

	void clear();

	void build(size_t num_keys, const key_t** keys);
	int  match(MatchMode mode,  const key_t* key, size_t length = 0, size_t* node_pos = 0) const;

private:
	void resize(size_t capacity);

protected:
	struct unit_t
	{
		int32 root : 1;		// ��ʶ�շ���ڵ�
		int32 leaf : 1;		// �Ƿ���Ҷ�ڵ㣬�������β
		int32 base : 22;	// ��ַ���ĳ�����Լ�λ��ƫ�Ƶ�ַ���ɼ��ٴ洢
		int32 label: 8;		// �ַ���ʶ
	};

	size_t	m_size;
	unit_t*	m_array;
};

CUTE_NS_END
