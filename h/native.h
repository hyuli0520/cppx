#pragma once

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>

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
		bool init();

		bool observe(socket* sock);
		void gqcs();

	private:
		HANDLE _cp;
	};
}