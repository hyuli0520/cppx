#include "socket.h"

socket::socket()
{
	_sock = INVALID_SOCKET;
}

socket::~socket()
{
}

void socket::close()
{
	if (_sock != INVALID_SOCKET)
	{
		closesocket(_sock);
		_sock = INVALID_SOCKET;
	}
}

void socket::create(protocol pt)
{
	auto type = socket_type::stream;
	if (pt == protocol::udp)
		type = socket_type::dgram;
	_sock = ::socket(PF_INET, static_cast<int>(type), static_cast<int>(pt));
}
