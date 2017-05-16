//! Importer
#include "Cute/Importer.h"
#include "Cute/File.h"
#include "Cute/Path.h"

CUTE_NS_BEGIN

Importer::Importer()
{
}

Importer::~Importer()
{
}

Resource* Importer::import(const String& filePath, const ImportOptions* options /* = NULL */, const String& uuid /* = Strings::BLANK */)
{
	if (!File::isFile(filePath))
	{
		return NULL;
	}

	Path path(filePath);
	String ext = path.getExtension();

	ImporterBase* importer = getImporter(ext);
	if (importer == NULL)
		return NULL;

	if (options == NULL)
		options = importer->getDefaultOptions();

	Resource* result = importer->import(filePath, options);

	return result;
}

ImporterBase* Importer::getImporter(const String& ext)
{
	ImporterBase* importer;
	for (ImporterList::reverse_iterator itor = m_importers.rbegin(); itor != m_importers.rend(); ++itor)
	{
		importer = *itor;
		if (importer->isExtensionSupported(ext))
			return importer;
	}

	return NULL;
}

CUTE_NS_END
