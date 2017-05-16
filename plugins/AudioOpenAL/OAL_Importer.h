#include "OAL_API.h"
#include "Cute/ImporterBase.h"

CUTE_NS_BEGIN

class CUTE_OAL_API OALImporter : public ImporterBase
{
public:
	OALImporter();
	~OALImporter();

	bool isExtensionSupported(const String& ext) const OVERRIDE;
	bool isMagicNumberSupported(uint32 magic) const OVERRIDE;

	ImportOptions* newOptions() const OVERRIDE;

	Resource* import(const String& path, const ImportOptions* options /* = NULL */) OVERRIDE;

};

CUTE_NS_END
