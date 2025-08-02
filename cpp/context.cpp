#include "context.h"

using namespace cppx;

context::context() :
#ifdef _WIN32 
	OVERLAPPED(),
#endif
	completed_callback([](context*, bool) {}), _io_type(io_type::none)
{
	init();
}

context::~context()
{
}

void context::init()
{
	_io_type = io_type::none;
#ifdef _WIN32
	ZeroMemory(static_cast<OVERLAPPED*>(this), sizeof(OVERLAPPED));
#endif
}
