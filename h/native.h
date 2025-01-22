#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <mswsock.h>

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
		static LPFN_ACCEPTEX accept;
		static LPFN_CONNECTEX connect;
		static LPFN_DISCONNECTEX disconnect;


	public:
		static bool init(int num);
		static bool bind_windows_function(SOCKET sock, GUID guid, LPVOID* fn);

		static void start_io(int num);

		static bool observe(socket* sock);
		static void gqcs();
		static bool process(context* context);

	private:
		static HANDLE _cp;
	};
}