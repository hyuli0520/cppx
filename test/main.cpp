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
		context context;
		context._buffer = vector<char>(buf, buf + 100);
		int receive = client.recv(&context);
// 	vector<char> msg(100, 0);
// 	int receive = client.recv(msg);
		cout << "connected" << endl;
		cout << "receive bytes : " << receive << endl;
		cout << "recv : " << context._buffer.data() << endl;
	}
}