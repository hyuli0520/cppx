#pragma once

#include "native.h"

struct host_entry
{
	string host_name;
};

class dns
{
public:
	static string get_host_name();
	static host_entry get_host_entry(string host);
};

