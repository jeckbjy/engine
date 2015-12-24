#include "File.h"

CU_NS_BEGIN

#ifdef WIN32
#define tfopen _wfopen
#else
#define tfopen fopen
#endif

File::File()
{

}

File::File(const String& path, int mode)
{
	open(path, mode);
}

File::~File()
{
	close();
}

bool File::open(const String& path, int mode)
{
	close();
	m_handle = tfopen(0, 0);
	return true;
}

void File::close()
{
#ifdef ANDROID
#else

#endif
}

void File::flush()
{
	if (m_handle)
		fflush(m_handle);
}

void File::seek(long pos)
{

}

uint File::read(void* dst, size_t len)
{
	return 0;
}

uint File::write(const void* data, size_t len)
{
	return 0;
}

bool File::is_open() const
{
	return m_handle != 0;
}

CU_NS_END
