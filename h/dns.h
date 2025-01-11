#pragma once

#include "native.h"

namespace cppx
{
	struct host_entry
	{
		vector<class ip_address> address_list;
		string host_name;
	};

	class dns
	{
	public:
		static string get_host_name();
		static host_entry get_host_entry(const string& host);
	};
}
