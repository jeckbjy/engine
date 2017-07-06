//! FileSystem
#include "Cute/Paths.h"
#include "Cute/Path.h"

CUTE_NS_BEGIN

String Paths::configHome()
{
    return Path::configHome();
}

String Paths::dataHome()
{
    return Path::dataHome();
}

String Paths::tempHome()
{
    return Path::tempHome();
}

String Paths::cacheHome()
{
    return Path::cacheHome();
}

String Paths::current()
{
    return Path::current();
}

String Paths::home()
{
    return Path::home();
}

String Paths::temp()
{
    return Path::temp();
}

String Paths::config()
{
    return Path::config();
}

String Paths::null()
{
    return Path::null();
}

// String Paths::getTempPath()
// {
//     return "";
// }

// String Paths::getTempFileName()
// {
//     return "";
// }

// String Paths::getRandomFileName()
// {
//     return "";
// }

String Paths::combie(const String& path1, const String& path2)
{
    return path1 + path2;
}

String Paths::getNode(const String& path)
{
    Path temp(path);
    return temp.getNode();
}

String Paths::getDevice(const String& path)
{
    Path temp(path);
    return temp.getDevice();
}

String Paths::getVersion(const String& path)
{
    Path temp(path);
    return temp.version();
}

String Paths::getAbsolute(const String& path)
{
    Path temp(path);
    return temp.absolute().toString();
}

String Paths::getDirectory(const String& path)
{
    Path temp(path);
    temp.makeDirectory();
    return temp.parent().toString();
}

// String Paths::getRoot(const String& path)
// {
//     Path temp(path);
//     return temp.getDevice();
// }

String Paths::getExtension(const String &path)
{
    Path temp(path);
    return temp.getExtension();
}

String Paths::getFileName(const String &path)
{
    Path temp(path);
    temp.makeFile();
    return temp.getFileName();
}

String Paths::getBaseFileName(const String& path)
{
    Path temp(path);
    temp.makeFile();
    return temp.getBaseName();
}

bool Paths::isFile(const String& path)
{
    Path temp(path);
    return temp.isFile();
}

bool Paths::isDirectory(const String& path)
{
    Path temp(path);
    return temp.isDirectory();
}

bool Paths::isAbsolute(const String& path)
{
    Path temp(path);
    return temp.isAbsolute();
}

bool Paths::isRelative(const String& path)
{
    Path temp(path);
    return temp.isRelative();
}

bool Paths::hasExtension(const String& path)
{
    return !getExtension(path).empty();
}

CUTE_NS_END
