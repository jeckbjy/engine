#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// deterministic finite automaton ，DFA
// c:    https://linux.thai.net/~thep/datrie/datrie.html
// c++:	 https://github.com/s-yata/darts-clone
// java: https://github.com/komiya-atsushi/darts-java
// java: http://www.cnblogs.com/zhangchaoyang/articles/4508266.html
// todo:Aho Corasick多模式匹配
/*
array[0]:存储的root信息，base:可以为负数,但是这样就不能动态添加key
可以支持utf8，gb2312等编码，要求兼容ascii，字符中间不能出现0
TODO:
1:Aho Corasick多模式匹配
2:tail压缩
3:unit_t压缩
4:动态添加字符
5:代码优化，类型强转改成typedef等
6:多字节优化支持
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

	enum Flag
	{
		FLAG_SORTED  = 0x01,	// 已经排序过
		FLAG_DYNAMIC = 0x02,	// 需要动态插入
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
		uint32 word : 1;	// 是否是单词结尾,叶子节点\0
		int32 base : 31;
		int32 check;
	};

	size_t	m_size;
	unit_t*	m_array;
};

CUTE_NS_END
