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

bool socket::not_invalid()
{
	return INVALID_SOCKET != _sock;
}

SOCKET socket::get_handle() const
{
	return _sock;
}

void socket::set_handle(SOCKET sock)
{
	_sock = sock;
}

void socket::set_endpoint(endpoint ep)
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

cppx::socket socket::accept() const
{
	socket client;
	client.set_handle(::accept(_sock, nullptr, nullptr));

	return client;
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

	if (!native::accept(_sock, context->_socket->get_handle(), buf, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dwBytes, reinterpret_cast<LPOVERLAPPED>(context)))
	{
		const auto error = WSAGetLastError();
		return error == WSA_IO_PENDING;
	}

	return true;
}

bool socket::connect(context* context)
{
	if (!context)
		return false;

	context->init();
	context->_io_type = io_type::connect;
	_endpoint = context->endpoint;

	if (!bind(endpoint(ip_address::any, 7777)))
		return false;

	DWORD dwBytes;
	ip_address ipAddr = context->endpoint->get_address();

	if (!native::connect(_sock, reinterpret_cast<sockaddr*>(&ipAddr), sizeof(sockaddr_in), nullptr, NULL, &dwBytes, reinterpret_cast<LPOVERLAPPED>(context)))
	{
		const auto error = WSAGetLastError();
		return error == WSA_IO_PENDING;
	}

	return true;
}

bool socket::connect(endpoint ep)
{
	if (!not_invalid())
		return false;

	if (!native::observe(this))
		return false;

	_endpoint = make_shared<endpoint>(ep);
	ip_address ipAddr = ep.get_address();
	return SOCKET_ERROR != ::connect(_sock, reinterpret_cast<sockaddr*>(&ipAddr), sizeof(ip_address));
}

bool socket::disconnect(context* context)
{
	if (!context)
		return false;

	context->init();
	context->_io_type = io_type::disconnect;
	_endpoint = nullptr;

	DWORD flag = 0;
	if (!native::disconnect(_sock, reinterpret_cast<LPOVERLAPPED>(context), flag, NULL))
	{
		const auto error = WSAGetLastError();
		return error == WSA_IO_PENDING;
	}

	return true;
}

bool socket::send(context* context)
{
	if (!context)
		return false;

	context->init();
	context->_io_type = io_type::send;

	WSABUF wsaBuf;
	wsaBuf.len = static_cast<ULONG>(context->_buffer.size());
	wsaBuf.buf = context->_buffer.data();

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_sock, &wsaBuf, 1, &numOfBytes, 0, reinterpret_cast<LPWSAOVERLAPPED>(context), NULL))
	{
		auto ret = WSAGetLastError();
		return ret == WSA_IO_PENDING;
	}

	return true;
}

bool socket::send(vector<char> msg)
{
	return SOCKET_ERROR != ::send(_sock, msg.data(), static_cast<int>(msg.size()), 0);
}

bool socket::recv(context* context)
{
	if (!context)
		return false;

	context->init();
	context->_io_type = io_type::receive;

	WSABUF wsaBuf;
	wsaBuf.len = static_cast<ULONG>(context->_buffer.size());
	wsaBuf.buf = context->_buffer.data();

	DWORD flag = 0;
	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSARecv(_sock, &wsaBuf, 1, &numOfBytes, &flag, reinterpret_cast<LPWSAOVERLAPPED>(context), NULL))
	{
		auto ret = WSAGetLastError();
		return ret == WSA_IO_PENDING;
	}

	return true;
}

int socket::recv(vector<char>& msg)
{
	auto result = ::recv(_sock, msg.data(), static_cast<int>(msg.size()), 0);
	return result;
}

int cppx::socket::recv(char* buf, size_t len)
{
	return ::recv(_sock, buf, static_cast<int>(len), 0);
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

bool socket::set_recv_buffer(int size)
{
	return set_option(SOL_SOCKET, SO_RCVBUF, size);
}

bool socket::set_send_buffer(int size)
{
	return set_option(SOL_SOCKET, SO_SNDBUF, size);
}

bool socket::set_tcp_nodelay(bool flag)
{
	return set_option(SOL_SOCKET, TCP_NODELAY, flag);
}

bool socket::set_update_accept(socket listen_sock)
{
	return set_option(SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listen_sock);
}