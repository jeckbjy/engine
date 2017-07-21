#pragma once
#include "FreeType_API.h"
#include "Cute/ImporterSpecific.h"

CUTE_NS_BEGIN

class CUTE_FREETYPE_API FreeTypeImporter : public ImporterSpecific
{
public:
	FreeTypeImporter();
	~FreeTypeImporter();

	Resource* import(const String& path, const ImportOptions* options /* = NULL */) OVERRIDE;
};

CUTE_NS_END
