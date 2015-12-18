#include <iostream>
using namespace std;

class Test
{
public:
	Test(){}
	virtual void handle(){};
};

int main(int argc, char** argv)
{
	cout << std::is_abstract<Test>::value << endl;
	system("pause");
	return 0;
}
