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
	if (!sock.bind(endpoint(ip_address::any, 7777)))
		return 0;
	if (!sock.listen())
		return 0;

	while (true)
	{
		auto client = sock.accept();
		cout << "connected" << endl;
	}
}