#include "context.h"

using namespace cppx;

context::context() : OVERLAPPED()
{
	init();
}

context::~context()
{
}

void cppx::context::init()
{
	::memset(this, 0, sizeof(OVERLAPPED));
	_io_type = io_type::none;
}
