#include "Object.h"
#include "Test.h"
#include "Config.h"

int main(int argc, char** argv)
{
	Config cfg;
	bool result = cfg.load("server.cfg");
	cout << result << std::endl;
	cout << cfg.toString() << std::endl;
	system("pause");
	return 0;
}
