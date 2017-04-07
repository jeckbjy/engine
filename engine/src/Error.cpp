#include "Cute/Error.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

error_t Error::last()
{
#ifdef _WIN32
	return ::GetLastError();
#else
	return errno;
#endif
}

String Error::getMessage(error_t errorCode)
{
#ifdef _WIN32
	String msg;
	DWORD flag = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
#ifdef CUTE_WIN32_UTF8
	LPWSTR buf = 0;
	if (FormatMessageW(flag, 0, errorCode, 0, (LPWSTR)& buf, 0, NULL))
		ConvertWideToUTF8(buf, msg);
#else
	LPTSTR buf = 0;
	if (FormatMessageA(flag, 0, errorCode, 0, (LPTSTR)& buf, 0, NULL))
		msg = buf;
#endif
	LocalFree(buf);

	return msg;
#else
	/* Reentrant version of `strerror'.
	There are 2 flavors of `strerror_r', GNU which returns the string
	and may or may not use the supplied temporary buffer and POSIX one
	which fills the string into the buffer.
	To use the POSIX version, -D_XOPEN_SOURCE=600 or -D_POSIX_C_SOURCE=200112L
	without -D_GNU_SOURCE is needed, otherwise the GNU version is
	preferred.
	*/
#	if (defined __GLIBC__ || defined __UCLIBC__) && defined _GNU_SOURCE && !__ANDROID__
	char errmsg[256] = "";
	return std::string(strerror_r(errorCode, errmsg, 256));
#	elif (_XOPEN_SOURCE >= 600) || __ANDROID__
	char errmsg[256] = "";
	strerror_r(errorCode, errmsg, 256);
	return errmsg;
#	else
	return std::string(strerror(errorCode));
#	endif
#endif
}

String Error::getMessage()
{
	return getMessage(last());
}

bool Error::isLast(error_t ec)
{
	return last() == ec;
}

void Error::emit(const String& info)
{
	emit(last(), info);
}

void Error::emit(error_t ec, const String& info)
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	switch (ec)
	{
	case ERROR_FILE_NOT_FOUND:
		throw FileNotFoundException(info, ec);
	default:
		throw Exception(info, ec);
	}
#else
	switch (ec)
	{
	case EIO:
		throw IOException(info, ec);
	default:
		throw Exception(info, ec);
	}
#endif
}

CUTE_NS_END
