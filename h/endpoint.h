#pragma once

#include "native.h"
#include "ip_address.h"

namespace cppx
{
	class endpoint
	{
	public:
		endpoint() = default;
		endpoint(ip_address address, u_short port);

		ip_address get_address() const;
		void set_address(ip_address address);
		u_short get_port();
		void set_port(u_short port);

		static endpoint place(sockaddr_in addr);

	private:
		ip_address _address;
	};
}
