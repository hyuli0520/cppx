#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#ifdef _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <mswsock.h>
#else
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

#define SOCKET_ERROR -1;
using SOCKET = int;

#endif
#include <thread>
#include <vector>
using namespace std;

namespace cppx
{
	class socket;
	class context;

	class native
	{
	public:
	#ifdef _WIN32
		static LPFN_ACCEPTEX accept;
		static LPFN_CONNECTEX connect;
		static LPFN_DISCONNECTEX disconnect;
	#endif

	public:
		static bool init(int num);
	#ifdef _WIN32
		static bool bind_windows_function(SOCKET sock, GUID guid, LPVOID* fn);
	#endif
		static void start_io(int num);

		static bool observe(socket* sock);
		static void gqcs();
		static bool process(context* context, unsigned long numofBytes, bool success);

	private:
	#ifdef _WIN32
		static HANDLE _cp;
	#endif
	};
}