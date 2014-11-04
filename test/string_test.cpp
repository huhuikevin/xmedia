#include <stdio.h>
#include <string>

int main()
{
	std::string test("http://\"nnnnbbbbaaaannnn\"ccccbbbbaaaa");
	size_t pos = test.find("\"");

	printf("pos=%d, %s\n", pos, test.substr(0, pos).c_str());

	size_t pos1 = test.find("\"", pos+1);
        
	printf("pos1 %d,  %s\n", pos1, test.substr(pos+1, pos1-pos-1).c_str());

	printf("-- %s --\n", (test.substr(pos1+1, (test.size()))+"END").c_str());

	return 0;
}
