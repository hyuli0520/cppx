#pragma once

#include "native.h"

class ip_address : public sockaddr_in
{
public:
	ip_address();
	ip_address(const sockaddr_in addr_in);
};

