#pragma once

#include "native.h"

namespace cppx
{
	enum class io_type
	{
		none,
		accept,
		connect,
		disconnect,
		send,
		receive
	};

	class context : OVERLAPPED
	{
	public:
		context();
		~context();

	public:
		vector<char> _buffer;

		io_type _io_type;
	};
}
