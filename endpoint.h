#pragma once

#include "native.h"
#include "ip_address.h"

class endpoint
{
public:
	endpoint() = default;
	endpoint(ip_address address, u_short port);
};

