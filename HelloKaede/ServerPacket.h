#ifndef __SERVERPACKET_H__
#define __SERVERPACKET_H__

#include<Windows.h>
#include<vector>


class ServerPacket {
private:
	std::vector<BYTE> packet;
	DWORD encoded;

public:
	ServerPacket(WORD wHeader);
	ServerPacket();

	std::vector<BYTE>& get();
	void SetHello();
	void Encode1(BYTE b);
	void Encode2(WORD w);
	void Encode4(DWORD dw);
	void Encode8(ULONGLONG ull);
	void EncodeStr(const char *s);
};

#endif