#include "native.h"
#include "socket.h"
#include "context.h"

using namespace cppx;

LPFN_ACCEPTEX native::accept = nullptr;
LPFN_CONNECTEX native::connect = nullptr;
LPFN_DISCONNECTEX native::disconnect = nullptr;

HANDLE native::_cp = nullptr;

bool native::init(int num)
{
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

	start_io(num);

	return true;
}

bool native::bind_windows_function(SOCKET sock, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(fn), &bytes, NULL, NULL);
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
	auto result = ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(sock->get_handle()), _cp, 0, 0);
	return result != nullptr;
}

void native::gqcs()
{
	context* context = nullptr;
	ULONG_PTR key = 0;
	DWORD numofBytes = 0;

	while (true)
	{
		auto result = ::GetQueuedCompletionStatus(_cp, &numofBytes, &key, reinterpret_cast<LPOVERLAPPED*>(&context), INFINITE);
		if (result)
		{
			process(context);
			break;
		}
		else
		{
			auto error = WSAGetLastError();
			switch (error)
			{
			case WAIT_TIMEOUT:
				return;
			default:
				process(context);
				break;
			}
		}
	}

}

bool native::process(context* context)
{
	switch (context->_io_type)
	{
	case io_type::accept:
	{
		socket listen_socket(protocol::tcp);
		if (!observe(context->_accept_socket.get()))
			return false;

		if(!context->_accept_socket->set_option(SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listen_socket.get_handle()))
			return false;

		sockaddr_in addr;
		int len = sizeof(sockaddr_in);
		if (::getpeername(context->_accept_socket->get_handle(), reinterpret_cast<sockaddr*>(&addr), &len) == SOCKET_ERROR)
			return false;
		auto endpoint = endpoint::set(addr);
		break;
	}
	case io_type::connect:
		break;
	case io_type::disconnect:
		break;
	case io_type::receive:
		break;
	case io_type::send:
		break;
	default:
		return false;
	}

	return true;
}
