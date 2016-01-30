#include "File.h"

CU_NS_BEGIN

#ifdef WIN32
//#define tfopen _wfopen
#define tfopen fopen
#else
#define tfopen fopen
#endif

File::File()
{

}

File::File(const String& path, int mode)
:m_handle(0)
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
	m_handle = tfopen(path.c_str(), "a+");
	if (m_handle)
	{
		seek(0, SEEK_END);
		m_size = position();
		seek(0, SEEK_SET);
	}
	return true;
}

void File::close()
{
#ifdef ANDROID
#else
	if (m_handle)
		fclose(m_handle);
#endif
}

void File::flush()
{
	if (m_handle)
		fflush(m_handle);
}

bool File::seek(long offset, int origin)
{
	return fseek(m_handle, offset, origin) != -1;
}

uint File::read(void* buf, uint size, uint count)
{
	return fread(buf, size, count, m_handle);
}

uint File::write(const void* buf, uint size, uint count)
{
	return fwrite(buf, size, count, m_handle);
}

uint File::position() const
{
	return ftell(m_handle);
}

bool File::is_open() const
{
	return m_handle != 0;
}

CU_NS_END
