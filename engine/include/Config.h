#pragma once
#include "API.h"

CU_NS_BEGIN

// config like toml and ini format
class CU_API Config
{
public:
	Config();
	~Config();

	bool load(const String& path);
	bool parse(const String& data);
private:
};

CU_NS_END