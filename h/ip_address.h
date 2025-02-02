#pragma once

#include "native.h"

namespace cppx
{
	class ip_address : public sockaddr_in
	{
	public:
		static ip_address any;
		static ip_address loopback;

	public:
		ip_address();
		ip_address(const sockaddr_in& addr_in);
		ip_address(const char* ip);

		static ip_address change(int ip);
	};
}