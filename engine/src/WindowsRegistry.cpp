//! Misc
#include "Cute/WindowsRegistry.h"
#include "Cute/Buffer.h"

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
                DWORD flags =  KEY_WRITE | KEY_QUERY_VALUE | mode;
                RegCreateKeyExA(rootKey, name.c_str(), 0, 0, REG_OPTION_NON_VOLATILE, flags, 0, &m_key, &result);
            }
            else
            {
                RegOpenKeyExA (rootKey, name.c_str(), 0, KEY_READ | mode, &m_key);
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
        if (name.istartsWith ("HKEY_CURRENT_USER\\"))
            return HKEY_CURRENT_USER;
        if (name.istartsWith("HKCU\\"))
            return HKEY_CURRENT_USER;
        if (name.istartsWith("HKEY_LOCAL_MACHINE\\"))
            return HKEY_LOCAL_MACHINE;
        if (name.istartsWith("HKLM\\"))
            return HKEY_LOCAL_MACHINE;
        if (name.istartsWith("HKEY_CLASSES_ROOT\\"))
            return HKEY_CLASSES_ROOT;
        if (name.istartsWith("HKCR\\"))
            return HKEY_CLASSES_ROOT;
        if (name.istartsWith("HKEY_USERS\\"))
            return HKEY_USERS;
        if (name.istartsWith("HKU\\"))
            return HKEY_USERS;
        return 0;
    }
    
    uint32 getBinaryValue(Buffer& result)
    {
        if(m_key == 0)
            return REG_NONE;
        
        for(DWORD bufferSize = 1024; ; bufferSize *= 2)
        {
            result.resize(bufferSize, false);
            DWORD type = REG_NONE;
			DWORD err = RegQueryValueExA(m_key, m_path.c_str(), 0, &type, (LPBYTE)result.data(), &bufferSize);
            if(err == ERROR_SUCCESS)
            {
                result.resize(bufferSize, false);
                return (uint32)type;
            }
            
            if(err != ERROR_MORE_DATA)
                break;
        }

		return REG_NONE;
    }

    String getValue(const String& defaultValue)
    {
        Buffer buffer;
        uint32 type = getBinaryValue(buffer);
        switch(type)
        {
        case REG_SZ:
            return static_cast<const CHAR*>(buffer.data());
        case REG_DWORD:
            return buffer.data();
        default:
            break;
        }
        
        return defaultValue;
    }

    bool setValue(const DWORD type,  const void* data, size_t size)
    {
        return RegSetValueExA(m_key, m_path.c_str(), 0, type, (const BYTE*)data, (DWORD)size) == ERROR_SUCCESS;
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

        DWORD result = RegQueryValueExA (m_key, m_path.c_str(), 0, &type, buffer, &bufferSize);

        return result == ERROR_SUCCESS || result == ERROR_MORE_DATA;
    }

    bool deleteValue()
    {
        if(m_key == 0)
            return false;
        return RegDeleteValueA (m_key, m_path.c_str()) == ERROR_SUCCESS;
    }

    bool deleteKey()
    {
        if(m_key == 0)
            return false;

        // todo:递归删除

        return RegDeleteKeyA (m_key, m_path.c_str()) == ERROR_SUCCESS;
    }

};

int32 WindowsRegistry::getValue(Buffer& result, const String& path, WoW64Mode mode)
{
    RegistryWrapper reg(path, false, mode);
    return reg.getBinaryValue(result);
}

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

bool WindowsRegistry::setValue(const String& path, const Buffer& value, WoW64Mode mode)
{
    RegistryWrapper reg(path, true, mode);
    return reg.setValue(REG_BINARY, value.data(), value.size());
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
