#include "Object.h"
#include <iostream>
using namespace cute;
using namespace std;

class Test : public Object
{
public:
	Test()
	{
		//addListener(Event::getRttiInstance(), &onClick);
	}
	void onClick(Event* ev);
};

int main(int argc, char** argv)
{
	system("pause");
	return 0;
}
