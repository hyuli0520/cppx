#include "context.h"

using namespace cppx;

context::context() : OVERLAPPED()
{
	_io_type = io_type::none;
}

context::~context()
{
}
