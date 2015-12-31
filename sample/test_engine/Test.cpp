#include "Test.h"

void test_buffer()
{
	Buffer buff;
	const char* aa = "asdfasdfsadf";
	const char* bb = "asdfasf";
	buff.write(aa, strlen(aa));
	buff.write(bb, strlen(bb));
	buff.seek(0, SEEK_SET);
	buff.seek(5, SEEK_CUR);
}