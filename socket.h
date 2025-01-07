#pragma once

#include "native.h"

enum class protocol
{
	tcp = IPPROTO_TCP,
	udp = IPPROTO_UDP
};

enum class address_family
{
	ipv4 = AF_INET,
	ipv6 = AF_INET6
};

enum class socket_type
{
	stream = SOCK_STREAM,
	dgram = SOCK_DGRAM
};

class socket
{
	socket();
	virtual ~socket();

private:
	SOCKET _sock;
};

