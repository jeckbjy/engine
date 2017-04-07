#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// todo:Aho Corasick多模式匹配
// deterministic finite automaton ，DFA
// c:    https://linux.thai.net/~thep/datrie/datrie.html
// c++:	 https://github.com/s-yata/darts-clone
// java: https://github.com/komiya-atsushi/darts-java
// java: http://www.cnblogs.com/zhangchaoyang/articles/4508266.html
// 如何支持中文？ tail
/*
array[0]:存储的root信息，base:可以为负数,但是这样就不能
todo:尚未完成
*/
class CUTE_CORE_API DoubleArrayTrie
{
public:
	enum MatchMode
	{
		MATCH_STRICT,		// 严格匹配
		MATCH_PREFIX_MIN,	// 最小前缀匹配
		MATCH_PREFIX_MAX,	// 最大前缀匹配
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
		int32 root : 1;		// 标识日否根节点
		int32 leaf : 1;		// 是否是叶节点，即词组结尾
		int32 base : 22;	// 基址，改成相对自己位置偏移地址，可减少存储
		int32 label: 8;		// 字符标识
	};

	size_t	m_size;
	unit_t*	m_array;
};

CUTE_NS_END
