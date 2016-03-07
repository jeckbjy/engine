#include "Test.h"
#include "XMLFile.h"

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

void test_xml()
{
	XMLFile file;
	bool res = file.open("./test.xml");
	if (!res)
		return;
	XMLNode root = file.getRoot();
	XMLNode node = root.getChild("rendertarget");
	while (node)
	{
		if (node.hasAttribute("name"))
			cout << "name:"<<node.getAttribute("name") << endl;
		node = node.getNext("rendertarget");
	}

	XMLNode cmdNode = root.getChild("command");
	while (cmdNode)
	{
		if (cmdNode.hasAttribute("name"))
			cout << "name:"<< cmdNode.getAttribute("name") << endl;
		cmdNode = cmdNode.getNext("command");
	}
}