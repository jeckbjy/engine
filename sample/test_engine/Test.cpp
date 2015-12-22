#include "Test.h"

void test_buffer()
{
	Buffer buff;
	const char* aa = "asdfasdfsadf";
	const char* bb = "asdfasf";
	buff.write(aa, strlen(aa));
	buff.write(bb, strlen(bb));
}