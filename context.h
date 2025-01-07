#pragma once

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
};

