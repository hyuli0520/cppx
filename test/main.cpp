#include <iostream>
using namespace std;

#include "native.h"
#include <socket.h>
using namespace cppx;

int main()
{
	native::init(5);

	cppx::socket sock(protocol::tcp);
	if (!sock.not_invalid())
		return -1;
	if (!sock.bind(endpoint(ip_address::any, 7777)))
		return -1;
	if (!sock.listen())
		return -1;

	while (true)
	{
		char buf[100] = { 0 };
		auto client = sock.accept();
		int receive = client.recv(buf, sizeof(buf));
		cout << "connected" << endl;
		cout << "receive bytes : " << receive << endl;
		cout << "recv : " << buf << endl;
	}
}