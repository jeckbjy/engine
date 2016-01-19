#include <iostream>
#include <cstdint>
using namespace std;

class Test
{
public:
	Test(){}
	virtual void handle(){};
};

int main(int argc, char** argv)
{
	//cout << std::is_abstract<Test>::value << endl;
	char* buf = "0x1111";
	int64_t a = 0;
	int count = sscanf(buf, "%liu", &a);
	cout << count<< "\t" << a << endl;
	system("pause");
	return 0;
}
