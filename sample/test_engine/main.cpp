#include "Object.h"
#include "Test.h"
#include "Config.h"
#include "Util.h"

int main(int argc, char** argv)
{
	//Config cfg;
	//bool result = cfg.load("server.cfg");
	//cout << result << std::endl;
	//cout << cfg.toString() << std::endl;

	String aa = "asfasdf\\a.txt";
	String bb = "asdf/b.txt";
	String ext1 = Util::getExtension(aa);
	String ext2 = Util::getExtension(bb);
	system("pause");
	return 0;
}
