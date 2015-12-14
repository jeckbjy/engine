#include <iostream>
using namespace std;

class Test
{
public:
	int a;
};

template<class T, size_t offset>
class TList
{

};

int main(int argc, char** argv)
{
	//TList<Test, offsetof(Test, a)> a;
	//cout << offsetof(Test, a) << endl;
	system("pause");
	return 0;
}
