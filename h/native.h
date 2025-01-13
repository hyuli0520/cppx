#pragma once

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
		bool init();
		bool bind_windows_function(SOCKET sock, GUID guid, LPVOID* fn);

		bool observe(socket* sock);
		void gqcs();

	private:
		HANDLE _cp;
	};
}