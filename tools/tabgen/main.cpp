#include "Cute/OptionSet.h"
#include "Cute/Table.h"
#include <fstream>
using namespace std;
using namespace Cute;

/*
1:自动生成c++代码
*/
int main(int argc, char* argv[])
{
	OptionSet options;
	options.parse(argc, argv);

	// 解析参数
	string tabfile;
	string outfile;
	int maxElement = 10;
	// 解析表头并输出类

	return 0;
}
