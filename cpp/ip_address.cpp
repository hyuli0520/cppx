#include "ip_address.h"

using namespace cppx;

ip_address ip_address::any = change(INADDR_ANY);
ip_address ip_address::loopback = change(INADDR_LOOPBACK);

ip_address::ip_address() : sockaddr_in()
{
	::memset(this, 0, sizeof(sockaddr_in));
	sin_family = AF_INET;
}

ip_address::ip_address(const sockaddr_in& addr_in) : sockaddr_in()
{
	sin_addr = addr_in.sin_addr;
	sin_family = AF_INET;
	sin_port = htons(addr_in.sin_port);
}

ip_address::ip_address(const char* ip)
{
	inet_pton(AF_INET, ip, &this->sin_addr);
	this->sin_family = AF_INET;
}

ip_address ip_address::change(int ip)
{
	ip_address addr;
	::memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_addr.s_addr = htonl(ip);
	return addr;
}
