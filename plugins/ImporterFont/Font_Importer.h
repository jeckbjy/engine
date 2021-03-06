#pragma once
#include "Font_API.h"
#include "Cute/ImporterSpecific.h"

CUTE_NS_BEGIN

class CUTE_FONT_API FontImporter : public ImporterSpecific
{
public:
	FontImporter();
	~FontImporter();

	Resource* import(const String& path, const ImportOptions* options /* = NULL */) OVERRIDE;
};

CUTE_NS_END
