#include "OAL_Importer.h"

CUTE_NS_BEGIN

OALImporter::OALImporter()
{
}

OALImporter::~OALImporter()
{
}

bool OALImporter::isExtensionSupported(const String& ext) const
{
	String lowerExt = toLower(ext);
	return lowerExt == "wav" || lowerExt == "flac" || lowerExt == "ogg";
}

bool OALImporter::isMagicNumberSupported(uint32 magic) const
{
	// Don't check for magic number, rely on extension
	return false;
}

ImportOptions* OALImporter::newOptions() const
{
	return NULL;
}

Resource* OALImporter::import(const String& path, const ImportOptions* options)
{
	return NULL;
}

CUTE_NS_END
