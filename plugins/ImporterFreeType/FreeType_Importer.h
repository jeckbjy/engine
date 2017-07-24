#pragma once
#include "FreeType_API.h"
#include "Cute/ImporterSpecific.h"

CUTE_NS_BEGIN

class CUTE_FREETYPE_API FreeType_Importer : public ImporterSpecific
{
public:
	FreeType_Importer();
	~FreeType_Importer();

	Resource* import(const String& path, const ImportOptions* options /* = NULL */) OVERRIDE;
};

CUTE_NS_END
