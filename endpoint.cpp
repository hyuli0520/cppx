#include "endpoint.h"

endpoint::endpoint(ip_address address, u_short port)
{
	_address = address;
	_address.sin_family = AF_INET;
	_address.sin_port = htons(port);
}

