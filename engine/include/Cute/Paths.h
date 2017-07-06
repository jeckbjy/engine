#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Paths
{
public:

    static String configHome();
    static String dataHome();
    static String tempHome();
    static String cacheHome();

    static String current();
    static String home();
    static String temp();
    static String config();
    static String null();

    // static String getTempPath();
    // static String getTempFileName();
    // static String getRandomFileName();

    static String combie(const String& path1, const String& path2);

    static String getNode(const String& path);
    static String getDevice(const String& path);
    // static String getRoot(const String& path);
    static String getVersion(const String& path);

    static String getAbsolute(const String& path);
    static String getDirectory(const String& path);
    static String getExtension(const String& path);
    static String getFileName(const String& path);
    static String getBaseFileName(const String& path);

    static bool isFile(const String& path);
    static bool isDirectory(const String& path);
    static bool isAbsolute(const String& path);
    static bool isRelative(const String& path);
    static bool hasExtension(const String& path);
};

CUTE_NS_END

