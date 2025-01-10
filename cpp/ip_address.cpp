#include "ip_address.h"

ip_address::ip_address() : sockaddr_in()
{
	::memset(this, 0, sizeof(sockaddr_in));
	sin_family = AF_INET;
}

ip_address::ip_address(const sockaddr_in addr_in) : sockaddr_in()
{
	sin_addr = addr_in.sin_addr;
	sin_family = AF_INET;
	sin_port = htons(addr_in.sin_port);
}
