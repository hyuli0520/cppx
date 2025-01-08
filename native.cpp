#include "native.h"

bool native::init()
{
    WSADATA wsaData;
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return false;

    _cp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}
