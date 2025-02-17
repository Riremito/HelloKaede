#include"Kyubey.h"

Kyubey::Kyubey(WORD Port, void (*thread)(Kyubey*)){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
	Server = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	// INADDR_LOOPBACK
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	bind(Server, (sockaddr *)&addr, sizeof(addr));
	listen(Server, 5);
	int len = sizeof(addr_client);
	while(1){
		Client = accept(Server, (struct sockaddr *)&addr_client, &len);
		Kyubey *NewClient = new Kyubey(Client);
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread, (LPVOID)NewClient, NULL, NULL);
		CloseHandle(hThread);
	}
}

Kyubey::Kyubey(char *Address, WORD Port){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
	Client = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	addr.sin_addr.S_un.S_addr = inet_addr(Address);
	connect(Client, (struct sockaddr *)&addr, sizeof(addr));
	Server = 0;
}

Kyubey::Kyubey(SOCKET NewClient){
	Client = NewClient;
	Server = 0;
}

Kyubey::~Kyubey(){
	if(Server){
		shutdown(Server, SD_BOTH);
		closesocket(Server);
		WSACleanup();
	}
	else{
		shutdown(Client, SD_BOTH);
		closesocket(Client);
	}
}

void Kyubey::SendPacket(ServerPacket &sp){
	send(Client, (char *)&sp.get()[0], sp.get().size(), 0);
}