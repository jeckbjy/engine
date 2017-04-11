#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// deterministic finite automaton ��DFA
// c:    https://linux.thai.net/~thep/datrie/datrie.html
// c++:	 https://github.com/s-yata/darts-clone
// java: https://github.com/komiya-atsushi/darts-java
// java: http://www.cnblogs.com/zhangchaoyang/articles/4508266.html
// todo:Aho Corasick��ģʽƥ��
/*
array[0]:�洢��root��Ϣ��base:����Ϊ����,���������Ͳ��ܶ�̬���key
����֧��utf8��gb2312�ȱ��룬Ҫ�����ascii���ַ��м䲻�ܳ���0
TODO:
1:Aho Corasick��ģʽƥ��
2:tailѹ��
3:unit_tѹ��
4:��̬����ַ�
5:�����Ż�������ǿת�ĳ�typedef��
6:���ֽ��Ż�֧��
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

	enum Flag
	{
		FLAG_SORTED  = 0x01,	// �Ѿ������
		FLAG_DYNAMIC = 0x02,	// ��Ҫ��̬����
		FLAG_ROOT1	 = 0x04,	// array[0].base==1
	};

	typedef char key_t;
	typedef const char* str_t;

	DoubleArrayTrie();
	~DoubleArrayTrie();

	bool open();
	void save();

	void clear();

	//void build(std::vector<String>& words, int flags);
	void build(size_t num_keys, const key_t** keys, int flags = 0);
	int  match(MatchMode mode,  const key_t* key, size_t length = 0, size_t* node_pos = 0) const;

private:
	void resize(size_t capacity);

protected:
	struct unit_t
	{
		uint32 word : 1;	// �Ƿ��ǵ��ʽ�β,Ҷ�ӽڵ�\0
		int32 base : 31;
		int32 check;
	};

	size_t	m_size;
	unit_t*	m_array;
};

CUTE_NS_END
