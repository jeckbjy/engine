#pragma once
#include "Cute/Foundation.h"
#include <sstream>

CUTE_NS_BEGIN

/*
�򵥵�Option����,���洢��Map��
Ҫ��
1:key����
2:�����ظ�
3:���:
 -:˵������Ĳ������ַ���ʽ,����ַ���Ҫ�ֿ�
--:˵������Ĳ����ǵ�����ʽ
��������:����=�ָ����߿ո����
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
