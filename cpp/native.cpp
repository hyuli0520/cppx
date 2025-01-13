#include "native.h"
#include "socket.h"
#include "context.h"

using namespace cppx;

bool native::init()
{
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	_cp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	return true;
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

				break;
			}
		}
	}
	
}
