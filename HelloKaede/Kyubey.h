#ifndef __KYUBEY_H__
#define __KYUBEY_H__

#include<winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include"ServerPacket.h"
#pragma warning(disable: 4996)


class Kyubey {
private:
	SOCKET Server, Client;
	sockaddr_in addr;
	sockaddr_in addr_client;
public:
	Kyubey(WORD Port, void (*thread)(Kyubey*));
	Kyubey(char *Address, WORD Port);
	Kyubey(SOCKET NewClient);
	~Kyubey();

	void SendPacket(ServerPacket &sp);
};

#endif