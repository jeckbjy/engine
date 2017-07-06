//! Misc
#include "Cute/WindowsRegistry.h"

CUTE_NS_BEGIN

#if defined(CUTE_OS_FAMILY_WINDOWS)

struct RegistryWrapper
{
    HKEY   m_key;
    String m_path;
    RegistryWrapper(String name, bool isWriting, WindowsRegistry::WoW64Mode mode)
    {
        if(HKEY rootKey = getRootKey(name))
        {
            // size_t pos = name.find('\')
            // name = name.substring(pos + 1);

            // size_t lastSlash = name.find();

            DWORD result;
            if(isWriting)
            {
                DWORD flags = REG_OPTION_NON_VOLATILE,KEY_WRITE | KEY_QUERY_VALUE | wow64Flags;
                RegCreateKeyEx(rootKey, name.c_str(), 0, 0, flags, 0, &m_key, &result);
            }
            else
            {
                RegOpenKeyEx (rootKey, name.c_str(), 0, KEY_READ | wow64Flags, &m_key);
            }
        }
    }

    ~RegistryWrapper()
    {
        if(m_key != 0)
            RegCloseKey(m_key);
    }

    HKEY getRootKey(const String& name) noexcept
    {
        if (name.startsWithIgnoreCase ("HKEY_CURRENT_USER\\"))
            return HKEY_CURRENT_USER;
        if (name.startsWithIgnoreCase ("HKCU\\"))
            return HKEY_CURRENT_USER;
        if (name.startsWithIgnoreCase ("HKEY_LOCAL_MACHINE\\"))
            return HKEY_LOCAL_MACHINE;
        if (name.startsWithIgnoreCase ("HKLM\\"))
            return HKEY_LOCAL_MACHINE;
        if (name.startsWithIgnoreCase ("HKEY_CLASSES_ROOT\\"))
            return HKEY_CLASSES_ROOT;
        if (name.startsWithIgnoreCase ("HKCR\\"))
            return HKEY_CLASSES_ROOT;
        if (name.startsWithIgnoreCase ("HKEY_USERS\\"))
            return HKEY_USERS;
        if (name.startsWithIgnoreCase ("HKU\\"))
            return HKEY_USERS;
        return 0;
    }

    String getValue(const String& defaultValue)
    {
        return "";
    }

    bool setValue(const DWORD type,  const void* data, size_t size)
    {
        return RegSetValueEx(m_key, m_path.c_str(), 0, type, (const BYTE*)data, (DWORD)size) == ERROR_SUCCESS;
    }

    bool keyExists()
    {
        return m_key != 0;
    }

    bool valueExists()
    {
        if(m_key == 0)
            return false;

        unsigned char buffer [512];
        unsigned long bufferSize = sizeof (buffer);
        DWORD type = 0;

        DWORD result = RegQueryValueEx (m_key, m_path.c_str(), 0, &type, buffer, &bufferSize);

        return result == ERROR_SUCCESS || result == ERROR_MORE_DATA;
    }

    bool deleteValue()
    {
        if(m_key == 0)
            return false;
        return RegDeleteValue (m_key, m_path.c_str()) == ERROR_SUCCESS;
    }

    bool deleteKey()
    {
        if(m_key == 0)
            return false;

        // todo:递归删除

        return RegDeleteKey(m_key, m_path.c_str()) == ERROR_SUCCESS;
    }

};

String WindowsRegistry::getValue(const String& path, const String& defaultValue, WoW64Mode mode)
{
    RegistryWrapper reg(path, false, mode);
    return reg.getValue(defaultValue);
}

bool WindowsRegistry::setValue(const String& path, const String& value, WoW64Mode mode)
{
    RegistryWrapper reg(path, true, mode);
    return reg.setValue(REG_SZ, value.data(), value.size());
}

bool WindowsRegistry::setValue(const String& path, uint32 value, WoW64Mode mode)
{
    RegistryWrapper reg(path, true, mode);
    return reg.setValue(REG_DWORD, &value, sizeof(value));
}

bool WindowsRegistry::setValue(const String& path, uint64 value, WoW64Mode mode)
{
    RegistryWrapper reg(path, true, mode);
    return reg.setValue(REG_QWORD, &value, sizeof(value));
}

bool WindowsRegistry::valueExists(const String& path, WoW64Mode mode)
{
    RegistryWrapper reg(path, false, mode);
    return reg.valueExists();
}

bool WindowsRegistry::keyExists(const String& path, WoW64Mode mode)
{
    RegistryWrapper reg(path, false, mode);
    return reg.keyExists();
}

bool WindowsRegistry::deleteValue(const String& path, WoW64Mode mode)
{
    RegistryWrapper reg(path, true, mode);
    return reg.deleteValue();
}

bool WindowsRegistry::deleteKey(const String& path, WoW64Mode mode)
{
    RegistryWrapper reg(path, true, mode);
    return reg.deleteKey();
}

bool WindowsRegistry::setFileAssociation(
                    const String& fileExtension,
                    const String& symbolicDescription,
                    const String& fullDescription,
                    const String& targetExecutable,
                    int   iconResourceNumber,
                    bool  registerForCurrentUserOnly,
                    WoW64Mode mode)
{
    String root = registerForCurrentUserOnly ? "HKEY_CURRENT_USER\\Software\\Classes\\" : "HKEY_CLASSES_ROOT\\";
    String key  = root + symbolicDescription;

    return setValue (root + fileExtension + "\\", symbolicDescription, mode)
        && setValue (key + "\\", fullDescription, mode)
        && setValue (key + "\\shell\\open\\command\\", targetExecutable + " \"%1\"", mode)
        && (iconResourceNumber == 0 || setValue (key + "\\DefaultIcon\\", targetExecutable + "," + String (iconResourceNumber)));
}

#endif

CUTE_NS_END