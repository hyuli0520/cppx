#include "native.h"
#include "socket.h"

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
