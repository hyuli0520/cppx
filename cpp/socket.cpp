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

bool socket::bind(endpoint ep)
{
	_endpoint = make_shared<endpoint>(ep);
	ip_address ipAddr = _endpoint->get_address();

	if(::bind(_sock, reinterpret_cast<sockaddr*>(&ipAddr), sizeof(sockaddr_in)) != SOCKET_ERROR)
		return true;

	return false;
}

bool socket::listen(int backlog)
{
	if (::listen(_sock, backlog) != SOCKET_ERROR)
		return true;

	return false;
}

bool socket::accept()
{
	sockaddr_in clientAddr;
	int addrLen = sizeof(sockaddr_in);

	if (::accept(_sock, (sockaddr*)&clientAddr, &addrLen) != INVALID_SOCKET)
		return true;

	return false;
}
