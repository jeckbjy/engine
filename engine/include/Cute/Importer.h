#pragma once
#include "Cute/Foundation.h"
#include "Cute/Module.h"
#include "Cute/String.h"
#include "Cute/ImporterSpecific.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Importer : public Module<Importer>
{
public:
	Importer();
	~Importer();

	Resource* import(const String& filePath, const ImportOptions* options = NULL, const String& uuid = Strings::BLANK);

private:
	ImporterSpecific* getImporter(const String& ext);
	
private:
	typedef std::vector<ImporterSpecific*> ImporterList;
	ImporterList m_importers;

};

CUTE_NS_END
