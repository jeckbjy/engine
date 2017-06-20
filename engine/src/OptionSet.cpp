//! Application
#include "Cute/OptionSet.h"
#include "Cute/Ascii.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

OptionSet::OptionSet()
{

}

OptionSet::~OptionSet()
{

}

void OptionSet::parse(int argc, char* argv[])
{
	/// The OptionProcessor supports two modes: Unix mode and default mode.
	/// In Unix mode, the option prefix is a dash '-'. A dash must be followed
	/// by a short option name, or another dash, followed by a (partial)
	/// long option name.
	/// In default mode, the option prefix is a slash '/', followed by 
	/// a (partial) long option name.
	/// If the special option '--' is encountered in Unix mode, all following
	/// options are ignored.
	///
	/// Option arguments can be specified in three ways. If a Unix short option
	/// ("-o") is given, the argument directly follows the option name, without
	/// any delimiting character or space ("-ovalue"). In default option mode, or if a
	/// Unix long option ("--option") is given, the option argument is 
	/// delimited from the option name with either an equal sign ('=') or
	/// a colon (':'), as in "--option=value" or "/option:value". Finally,
	/// a required option argument can be specified on the command line after the
	/// option, delimited with a space, as in "--option value" or "-o value".
	/// The latter only works for required option arguments, not optional ones.

	// 0是启动程序名,1开始是参数
	for (int i = 1; i < argc; ++i)
	{
		char* cmd = argv[i];
		char ch = cmd[0];

		if ((ch != '-') && (ch != '/'))
			throw Exception("bad command");

		char* ptr = cmd + 1;
		if (*ptr == 0)
			throw Exception("empty command!");

		if (*ptr == '-')
		{
			++ptr;
			if (ch != '-')
				throw Exception("bad format /- is forbid!!");
		}

		char* end_ptr = ptr;
		while (!Ascii::isAlpha(*end_ptr))
			break;

		if (end_ptr == ptr)
			throw Exception("empty option key");

		bool hasValue = (*end_ptr == '=') || (*end_ptr == ':');

		// short mode : like  tar -xzvf 
		if (!hasValue && ch == '-' && cmd[1] != '-')
		{
			while ((*ptr) != 0)
			{
				m_optionMap[String(*ptr, 1)] = "";
			}
		}
		else
		{
			String key(ptr, end_ptr);
			String val(hasValue ? end_ptr + 1 : "");

			// 如果没有值，检测下一个字段是否是值value
			if (!hasValue && (i + 1) < argc)
			{
				cmd = argv[1];
				if (cmd[0] != '-' && cmd[1] != '/')
				{
					++i;
					val = cmd;
				}
			}

			m_optionMap[key] = val;
		}
	}
}

bool OptionSet::has(const String& key) const
{
	return m_optionMap.find(key) != m_optionMap.end();
}

bool OptionSet::get(String& value, const String& key) const
{
	OptionMap::const_iterator itor = m_optionMap.find(key);
	if (itor != m_optionMap.end())
	{
		value = itor->second;
		return true;
	}
	else
	{
		return false;
	}
}

bool OptionSet::get(String& value, const String& key, const String& defaultValue /* = "" */) const
{
	OptionMap::const_iterator itor = m_optionMap.find(key);
	if (itor != m_optionMap.end())
	{
		value = itor->second;
		return true;
	}
	else
	{
		value = defaultValue;
		return false;
	}

}

CUTE_NS_END