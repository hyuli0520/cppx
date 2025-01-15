#include "socket.h"

using namespace cppx;

socket::socket()
{
	_sock = INVALID_SOCKET;
}

cppx::socket::socket(const socket& sock)
{
	_sock = sock._sock;
	_endpoint = sock._endpoint;
}

socket::~socket()
{
}

SOCKET socket::get_handle()
{
	return _sock;
}

void socket::set_handle(SOCKET sock)
{
	_sock = sock;
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

	if (::bind(_sock, reinterpret_cast<sockaddr*>(&ipAddr), sizeof(sockaddr_in)) != SOCKET_ERROR)
		return true;

	return false;
}

bool socket::listen(int backlog)
{
	if (::listen(_sock, backlog) != SOCKET_ERROR)
		return true;

	return false;
}

bool socket::accept(context* context)
{
	if (!context)
		return false;

	context->init();
	context->_io_type = io_type::accept;
	context->_accept_socket = make_shared<socket>(protocol::tcp);

	DWORD dwBytes;
	char buf[1024];

	if (!native::accept(_sock, context->_accept_socket->get_handle(), buf, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dwBytes, reinterpret_cast<LPOVERLAPPED>(&context)))
	{
		const auto error = WSAGetLastError();
		return error == WSA_IO_PENDING;
	}

	return true;
}
