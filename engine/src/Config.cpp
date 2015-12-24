#include "Config.h"
#include "File.h"

CU_NS_BEGIN

Config::Config()
{

}

Config::~Config()
{

}

bool Config::load(const String& path)
{
	File file(path);
	if (!file.is_open())
		return false;
	String data;
	data.resize(file.size());
	file.read(&data[0], file.size());
	return parse(data);
}

bool Config::parse(const String& data)
{
	return true;
}

CU_NS_END