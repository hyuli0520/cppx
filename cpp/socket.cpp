#include "socket.h"

using namespace cppx;

socket::socket()
{
	_sock = INVALID_SOCKET;
}

socket::socket(const socket& sock)
{
	_sock = sock._sock;
	_endpoint = sock._endpoint;
}

socket::socket(protocol pt)
{
	create(pt);
}

socket::~socket()
{
	close();
}

SOCKET socket::get_handle() const
{
	return _sock;
}

void socket::set_handle(SOCKET sock)
{
	_sock = sock;
}

void cppx::socket::set_endpoint(endpoint ep)
{
	_endpoint = make_shared<endpoint>(ep);
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

bool socket::listen(int backlog) const
{
	return SOCKET_ERROR != ::listen(_sock, backlog);
}

bool socket::accept(context* context)
{
	if (!context)
		return false;

	context->init();
	context->_io_type = io_type::accept;
	context->_socket = make_shared<socket>(protocol::tcp);

	DWORD dwBytes;
	char buf[1024];

	if (!native::accept(_sock, context->_socket->get_handle(), buf, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dwBytes, reinterpret_cast<LPOVERLAPPED>(&context)))
	{
		const auto error = WSAGetLastError();
		return error == WSA_IO_PENDING;
	}

	return true;
}

bool socket::set_linger(short onoff, short linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return set_option(SOL_SOCKET, SO_LINGER, option);
}

bool socket::set_reuse_address(bool flag)
{
	return set_option(SOL_SOCKET, SO_REUSEADDR, flag);
}

bool cppx::socket::set_recv_buffer(int size)
{
	return set_option(SOL_SOCKET, SO_RCVBUF, size);
}

bool cppx::socket::set_send_buffer(int size)
{
	return set_option(SOL_SOCKET, SO_SNDBUF, size);
}

bool cppx::socket::set_tcp_nodelay(bool flag)
{
	return set_option(SOL_SOCKET, TCP_NODELAY, flag);
}

bool cppx::socket::set_update_accept(socket listen_sock)
{
	return set_option(SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listen_sock);
}