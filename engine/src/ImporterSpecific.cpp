//! Importer
#include "Cute/ImporterSpecific.h"

CUTE_NS_BEGIN

bool ImporterSpecific::isExtensionSupported(const String& ext) const
{
	const char** extensions = getExtensionList();
	int index = 0;
	for (;;)
	{
		const char* tmp = extensions[index++];
		if (tmp == NULL)
			break;

		if (strcasecmp(ext.c_str(), tmp) == 0)
			return true;
	}

	return false;
}

CUTE_NS_END
