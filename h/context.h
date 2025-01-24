#pragma once

#include "native.h"
#include "endpoint.h"

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

		void init();

	public:
		shared_ptr<endpoint> endpoint;

	public:
		shared_ptr<socket> _socket;
		vector<char> _buffer;

		io_type _io_type;
	};
}
