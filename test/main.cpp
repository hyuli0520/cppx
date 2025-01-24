#include <iostream>
using namespace std;

#include "native.h"
#include <socket.h>
using namespace cppx;

int main()
{
	native::init(5);

	cppx::socket sock(cppx::protocol::tcp);
	if (!sock.not_invalid())
		return 0;
	return 0;
}