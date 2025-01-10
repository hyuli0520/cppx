#pragma once

#include "native.h"
#include "endpoint.h"

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
public:
	socket();
	virtual ~socket();

	void close();
	void create(protocol pt = protocol::tcp);

	bool bind(endpoint ep);
	bool listen(int backlog);

	bool accept();

private:
	shared_ptr<endpoint> _endpoint;

	SOCKET _sock;
};

