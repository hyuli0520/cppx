#include "native.h"
#include "socket.h"
#include "context.h"

using namespace cppx;

#ifdef _WIN32
LPFN_ACCEPTEX native::accept = nullptr;
LPFN_CONNECTEX native::connect = nullptr;
LPFN_DISCONNECTEX native::disconnect = nullptr;

bool native::bind_windows_function(SOCKET sock, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), &bytes, NULL, NULL);
}

HANDLE native::_cp = nullptr;
#else
bool native::make_non_blocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
	{
		perror("fcntl");
		return -1;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		perror("fcntl");
		return -1;
	}
	
	return 0;
}
#endif

bool native::init(int num)
{
#ifdef _WIN32
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	socket dummy(protocol::tcp);
	if (!bind_windows_function(dummy.get_handle(), WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&accept)))
		return false;
	if (!bind_windows_function(dummy.get_handle(), WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&connect)))
		return false;
	if (!bind_windows_function(dummy.get_handle(), WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&disconnect)))
		return false;

	_cp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
#else
	_epfd = epoll_create1(0);
	if (_epfd == -1)
	{
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}

	if (make_non_blocking(STDIN_FILENO) == -1)
	{
		close(_epfd);
		exit(EXIT_FAILURE);
	}
	
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = STDIN_FILENO;
#endif
	start_io(num);

	return true;
}

void native::start_io(int num)
{
	for (int i = 0; i < num; i++)
	{
		new thread(&gqcs);
	}
}

bool native::observe(socket* sock)
{
#ifdef _WIN32
	auto result = ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(sock->get_handle()), _cp, 0, 0);
	return result != nullptr;
#else
	return true;
#endif
}

void native::gqcs()
{
	context* context = nullptr;
#ifdef _WIN32
	ULONG_PTR key = 0;
	DWORD numofBytes = 0;
#endif
	while (true)
	{
	#ifdef _WIN32
		auto result = ::GetQueuedCompletionStatus(_cp, &numofBytes, &key, reinterpret_cast<LPOVERLAPPED*>(&context), INFINITE);
		
		if (result)
		{
			process(context, numofBytes, true);
		}
		else
		{
			auto error = WSAGetLastError();
			switch (error)
			{
				case WAIT_TIMEOUT:
				return;
				default:
				process(context, numofBytes, false);
				break;
			}
		}
	#else
		
	#endif
	}
}

bool native::process(context* context, unsigned long numofBytes, bool success)
{
	switch (context->_io_type)
	{
	case io_type::accept:
		if (success)
		{
			auto listen_socket = context->_socket.get();
		#ifdef _WIN32
			if (!observe(context->_socket.get()))
				return false;

			if (!context->_socket->set_option(SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listen_socket->get_handle()))
				return false;
		#endif
			sockaddr_in addr;
			socklen_t len = sizeof(sockaddr_in);
			if (::getpeername(context->_socket->get_handle(), reinterpret_cast<sockaddr*>(&addr), &len) == SOCKET_ERROR)
				return false;
			auto endpoint = endpoint::place(addr);
			context->_socket->set_endpoint(endpoint);
		}
		context->completed_callback(context, success);
		break;
	case io_type::connect:
		if (success)
		{
		#ifdef _WIN32
			if (!context->_socket->set_option(SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, nullptr))
				return false;
		#endif
		}
		context->completed_callback(context, success);
		break;
	case io_type::disconnect:
		context->completed_callback(context, success);
		break;
	case io_type::receive:
		if (numofBytes == 0)
			success = false;
	case io_type::send:
		context->length = numofBytes;
		context->completed_callback(context, success);
		break;
	default:
		return false;
	}

	return true;
}
