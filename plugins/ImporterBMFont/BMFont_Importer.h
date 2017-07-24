#pragma once
#include "BMFont_API.h"
#include "Cute/ImporterSpecific.h"

CUTE_NS_BEGIN

class CUTE_BMFONT_API BMFont_Importer : public ImporterSpecific
{
public:
	BMFont_Importer();
	~BMFont_Importer();

	Resource* import(const String& path, const ImportOptions* options /* = NULL */) OVERRIDE;
};

CUTE_NS_END
