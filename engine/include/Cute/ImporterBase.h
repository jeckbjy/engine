//! Importer
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// resource asset
class CUTE_CORE_API Resource : public Object
{

};

class CUTE_CORE_API ImportOptions : public Object
{
	DECLARE_RTTI(ImportOptions, Object, OID_ANY)
public:
	virtual ~ImportOptions(){}
};

// ImporterSpecific
class CUTE_CORE_API ImporterBase : public Object
{
	DECLARE_RTTI(ImporterBase, Object, OID_ANY)
public:
	virtual ~ImporterBase(){}

	virtual bool isExtensionSupported(const String& ext) const = 0;
	virtual bool isMagicNumberSupported(uint32 magic) const = 0;

	virtual Resource* import(const String& path, const ImportOptions* options = NULL) = 0;

	virtual ImportOptions*			newOptions() const = 0;
	virtual const ImportOptions*	getDefaultOptions() const = 0;

private:
};

CUTE_NS_END
