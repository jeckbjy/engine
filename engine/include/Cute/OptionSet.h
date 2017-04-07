#pragma once
#include "Cute/Foundation.h"
#include <sstream>

CUTE_NS_BEGIN

/*
简单的Option解析,并存储到Map中
要求：
1:key无序
2:不可重复
3:风格:
 -:说明后面的参数是字符形式,多个字符需要分开
--:说明后面的参数是单词形式
参数可用:或者=分隔或者空格紧跟
*/
class CUTE_CORE_API OptionSet
{
public:
	OptionSet();
	~OptionSet();

	void parse(int argc, char* argv[]);

	bool has(const String& key) const;
	bool get(String& value, const String& key) const;
	bool get(String& value, const String& key, const String& defaultValue) const;

	template<typename T>
	bool get(T& value, const String& key) const
	{
		OptionMap::const_iterator itor = m_optionMap.find(key);
		if (itor != m_optionMap.end() && !itor->second.empty())
		{
			std::stringstream ss(itor->second);
			ss >> value;
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename T>
	bool get(T& value, const String& key, const T& defaultValue) const
	{
		OptionMap::const_iterator itor = m_optionMap.find(key);
		if (itor != m_optionMap.end() && !itor->second.empty())
		{
			std::stringstream ss(itor->second);
			ss >> value;
			return true;
		}
		else
		{
			value = defaultValue;
			return false;
		}
	}

protected:
	typedef std::map<String, String>	OptionMap;
	OptionMap m_optionMap;
};

CUTE_NS_END
