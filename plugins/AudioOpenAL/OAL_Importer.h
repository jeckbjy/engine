#include "OAL_API.h"
#include "Cute/ImporterSpecific.h"

CUTE_NS_BEGIN

class CUTE_OAL_API OALImporter : public ImporterSpecific
{
public:
	OALImporter();
	~OALImporter();

	//bool isExtensionSupported(const String& ext) const OVERRIDE;
	//bool isMagicNumberSupported(uint32 magic) const OVERRIDE;
	const char**	getExtensionList() const OVERRIDE;
	ImportOptions*	newOptions() const OVERRIDE;

	Resource* import(const String& path, const ImportOptions* options /* = NULL */) OVERRIDE;

};

CUTE_NS_END
