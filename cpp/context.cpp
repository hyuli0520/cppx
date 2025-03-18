#include "context.h"

using namespace cppx;

context::context() : OVERLAPPED(), _io_type(io_type::none)
{
	init();
}

context::~context()
{
}

void context::init()
{
	_io_type = io_type::none;
	ZeroMemory(static_cast<OVERLAPPED*>(this), sizeof(OVERLAPPED));
}
