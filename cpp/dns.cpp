#include "dns.h"
#include "ip_address.h"

using namespace cppx;

string dns::get_host_name()
{
	char buf[128] = "";
	gethostname(buf, 128);
	return buf;
}

host_entry dns::get_host_entry(const string &host)
{
	struct addrinfo hints {}, *res = nullptr;
	host_entry entry;

	int status = getaddrinfo(host.c_str(), nullptr, &hints, &res);
	if (status != 0)
		return {};

	for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
		char ip[INET6_ADDRSTRLEN];

		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in* ipv4 = reinterpret_cast<struct sockaddr_in*>(p->ai_addr);
			inet_ntop(AF_INET, &(ipv4->sin_addr), ip, sizeof(ip));
		}
		else if (p->ai_family == AF_INET6) { // IPv6
			struct sockaddr_in6* ipv6 = reinterpret_cast<struct sockaddr_in6*>(p->ai_addr);
			inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, sizeof(ip));
		}
		else {
			continue;
		}

		entry.address_list.emplace_back(ip);
	}

	if (res && res->ai_canonname)
		entry.host_name = res->ai_canonname;

	freeaddrinfo(res);
	return entry;
}
