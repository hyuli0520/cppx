#pragma once

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#include <thread>
#include <vector>
using namespace std;

class native
{
public:
	bool init();

private:
	HANDLE _cp;
};

