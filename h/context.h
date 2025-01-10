#pragma once

#include "native.h"

enum class io_type
{
	none,
	accept,
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

public:
	vector<char> _buffer;

	io_type _io_type;
};

