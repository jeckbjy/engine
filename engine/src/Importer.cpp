//! Importer
#include "Cute/Importer.h"
#include "Cute/Files.h"
#include "Cute/Paths.h"

CUTE_NS_BEGIN

Importer::Importer()
{
}

Importer::~Importer()
{
}

Resource* Importer::import(const String& filePath, const ImportOptions* options /* = NULL */, const String& uuid /* = Strings::BLANK */)
{
	if (!Files::isFile(filePath))
	{
		return NULL;
	}

    String ext = Paths::getExtension(filePath);

	ImporterSpecific* importer = getImporter(ext);
	if (importer == NULL)
		return NULL;

	if (options == NULL)
		options = importer->getDefaultOptions();

	Resource* result = importer->import(filePath, options);

	return result;
}

ImporterSpecific* Importer::getImporter(const String& ext)
{
	ImporterSpecific* importer;
	for (ImporterList::reverse_iterator itor = m_importers.rbegin(); itor != m_importers.rend(); ++itor)
	{
		importer = *itor;
		if (importer->isExtensionSupported(ext))
			return importer;
	}

	return NULL;
}

CUTE_NS_END
