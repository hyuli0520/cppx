#include "endpoint.h"

using namespace cppx;

endpoint::endpoint(ip_address address, u_short port)
{
	_address = address;
	_address.sin_family = AF_INET;
	_address.sin_port = htons(port);
}

ip_address endpoint::get_address() const
{
	return _address;
}

void endpoint::set_address(ip_address address)
{
	_address = address;
}

u_short endpoint::get_port()
{
	return ntohs(_address.sin_port);
}

void endpoint::set_port(u_short port)
{
	_address.sin_port = htons(port);
}

endpoint endpoint::place(sockaddr_in addr)
{
	endpoint ep;
	ep.set_address(ip_address(addr));
	ep.set_port(addr.sin_port);

	return ep;
}
