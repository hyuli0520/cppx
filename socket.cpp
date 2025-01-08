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

bool socket::bind(int port, address_family af)
{
	sockaddr_in addr_in;
	addr_in.sin_family = static_cast<u_short>(af);
	addr_in.sin_port = port;
	addr_in.sin_addr.s_addr = htonl(INADDR_ANY);

	if(::bind(_sock, (sockaddr*)&addr_in, sizeof(sockaddr_in)) != SOCKET_ERROR)
		return true;

	return false;
}

bool socket::listen(int backlog)
{
	if (::listen(_sock, backlog) != SOCKET_ERROR)
		return true;

	return false;
}
