#include "native.h"
#include "socket.h"
#include "context.h"

using namespace cppx;

LPFN_ACCEPTEX native::accept = nullptr;
LPFN_CONNECTEX native::connect = nullptr;
LPFN_DISCONNECTEX native::disconnect = nullptr;

bool native::init()
{
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	socket dummy;
	dummy.create(protocol::tcp);
	if (!bind_windows_function(dummy.get_handle(), WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&accept)))
		return false;
	if (!bind_windows_function(dummy.get_handle(), WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&connect)))
		return false;
	if (!bind_windows_function(dummy.get_handle(), WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&disconnect)))
		return false;

	_cp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	return true;
}

bool cppx::native::bind_windows_function(SOCKET sock, GUID guid, LPVOID* fn)
{
	return false;
}

bool native::observe(socket* sock)
{
	auto result = ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(sock->get_handle()), _cp, 0, 0);
	return result != nullptr;
}

void cppx::native::gqcs()
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

bool cppx::native::process(context* context)
{
	switch (context->_io_type)
	{
	case io_type::accept:
		sockaddr_in addr;
		int len = sizeof(sockaddr_in);
		if (::getpeername(context->_accept_socket->get_handle(), reinterpret_cast<sockaddr*>(&addr), &len) == SOCKET_ERROR)
			return false;
		auto endpoint = endpoint::set(addr);
		break;
	case io_type::connect:
	case io_type::disconnect:
	case io_type::receive:
	case io_type::send:
	default:
		return false;
	}

	return true;
}
