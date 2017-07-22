#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN
#if defined(CUTE_OS_FAMILY_WINDOWS)

class Buffer;
class CUTE_CORE_API WindowsRegistry
{
public:
    /** These values can be used to specify whether the 32- or 64-bit registry should be used.
        When running on a 32-bit OS, there is no 64-bit registry, so the mode will be ignored.
    */
    enum WoW64Mode
    {
        /** Default handling: 32-bit apps will use the 32-bit registry, and 64-bit apps
            will use the 64-bit registry. */
        WoW64_Default = 0,

        /** Always use the 64-bit registry store. (KEY_WOW64_64KEY). */
        WoW64_64bit  = 0x100,

        /** Always use the 32-bit registry store. (KEY_WOW64_32KEY). */
        WoW64_32bit  = 0x200
    };

    static int32  getValue(Buffer& result, const String& path, WoW64Mode mode = WoW64_Default);
    static String getValue(const String& path, const String& defaultValue = String(), WoW64Mode mode = WoW64_Default);
    
    static bool   setValue(const String& path, const String& value, WoW64Mode mode = WoW64_Default);
    static bool   setValue(const String& path, uint32 value, WoW64Mode mode = WoW64_Default);
    static bool   setValue(const String& path, uint64 value, WoW64Mode mode = WoW64_Default);
    static bool   setValue(const String& path, const Buffer& value, WoW64Mode mode = WoW64_Default);
   
    static bool   valueExists(const String& path, WoW64Mode mode = WoW64_Default);
    static bool   keyExists(const String& path, WoW64Mode mode = WoW64_Default);
    static bool   deleteValue(const String& path, WoW64Mode mode = WoW64_Default);
    static bool   deleteKey(const String& path, WoW64Mode mode = WoW64_Default);
    static bool   setFileAssociation(
                    const String& fileExtension,
                    const String& symbolicDescription,
                    const String& fullDescription,
                    const String& targetExecutable,
                    int iconResourceNumber,
                    bool registerForCurrentUserOnly,
                    WoW64Mode mode = WoW64_Default);
};

#endif

CUTE_NS_END
