#pragma once

#include "native.h"
#include "endpoint.h"
#include "context.h"

namespace cppx
{
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
		socket(const socket& sock);
		socket(protocol pt);
		virtual ~socket();

		bool not_invalid() const;
		SOCKET get_handle() const;
		void set_handle(SOCKET sock);

		shared_ptr<endpoint> get_endpoint() const;
		void set_endpoint(endpoint ep);

		void close();
		void create(protocol pt = protocol::tcp);

		bool bind(endpoint ep);
		bool listen(int backlog = SOMAXCONN) const;

		socket accept() const;
		bool accept(context* context);
		bool connect(context* context);
		bool connect(endpoint ep);
		bool disconnect(context* context);
		bool disconnect();
		bool send(context* context);
		bool send(vector<char> msg) const;
		bool recv(context* context);
		int recv(vector<char>& msg) const;
		int recv(char* buf, size_t len) const;

		bool set_linger(short onoff, short linger);
		bool set_reuse_address(bool flag) const;
		bool set_recv_buffer(int size) const;
		bool set_send_buffer(int size) const;
		bool set_tcp_nodelay(bool flag) const;
		bool set_update_accept(socket listen_sock) const;


		template<typename t>
		bool set_option(int level, int opt_name, t opt_val) const
		{
			if (_sock == INVALID_SOCKET)
				return false;
			return SOCKET_ERROR != setsockopt(_sock, level, opt_name, reinterpret_cast<char*>(&opt_val), sizeof(t));
		};

	private:
		shared_ptr<endpoint> _endpoint;

		SOCKET _sock;
	};
}
