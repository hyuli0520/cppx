#include "dns.h"

string dns::get_host_name()
{
	char buf[128] = "";
	gethostname(buf, 128);
	return buf;
}

host_entry dns::get_host_entry(string host)
{
	auto entry = gethostbyname(host.c_str());
	if (entry == nullptr)
		return;

	host_entry hostEntry;
	hostEntry.host_name = entry->h_name;

	return hostEntry;
}
