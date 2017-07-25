#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Regex
{
public:
	Regex();
	~Regex();

private:
	// Note: to avoid a dependency on the pcre.h header the following are 
	// declared as void* and casted to the correct type in the implementation file.
	void* m_pcre;  // Actual type is pcre*
	void* m_extra; // Actual type is struct pcre_extra*
};

CUTE_NS_END
