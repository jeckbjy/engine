#pragma once
#include "BMFont_API.h"
#include "Cute/ImporterSpecific.h"

CUTE_NS_BEGIN

class CUTE_FONT_API BMFontImporter : public ImporterSpecific
{
public:
	BMFontImporter();
	~BMFontImporter();

	Resource* import(const String& path, const ImportOptions* options /* = NULL */) OVERRIDE;
};

CUTE_NS_END
