#include "context.h"

using namespace cppx;

context::context() : OVERLAPPED(), _accept_socket(nullptr), _io_type(io_type::none)
{
	init();
}

context::~context()
{
}

void context::init()
{
	_accept_socket.reset();
	_buffer.clear();
	_io_type = io_type::none;
	ZeroMemory(static_cast<OVERLAPPED*>(this), sizeof(OVERLAPPED));
}
