#include <iostream>
using namespace std;

#include "native.h"
using namespace cppx;

int main()
{
	native::init(5);

	cout << "input any key" << endl;
	(void)getchar();

	return 0;
}