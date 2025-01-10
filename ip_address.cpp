#include "ip_address.h"

ip_address::ip_address() : sockaddr_in()
{
	::memset(this, 0, sizeof(sockaddr_in));
	sin_family = AF_INET;
}
