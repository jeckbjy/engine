#pragma once
#include "Cute/ImporterSpecific.h"

CUTE_NS_BEGIN

class CUTE_CORE_API AudioImporter : public ImporterSpecific
{
public:
	AudioImporter();
	~AudioImporter();

	Resource*	import(const String& path, const ImportOptions* options = NULL) OVERRIDE;
	bool		isExtensionSupported(const String& ext) const OVERRIDE;
};

CUTE_NS_END
