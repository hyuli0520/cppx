#pragma once

#include "native.h"

enum class io_type
{
	none,
	connect,
	disconnect,
	send,
	receive
};

class context
{
public:
	context();
	~context();

private:
	vector<char> _buffer;

	io_type _io_type;
};

