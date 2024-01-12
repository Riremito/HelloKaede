#include"ServerPacket.h"

ServerPacket::ServerPacket(WORD wHeader) {
	encoded = 0;
	Encode2(wHeader);
}


ServerPacket::ServerPacket() {
	encoded = 0;
}

std::vector<BYTE>& ServerPacket::get() {
	return packet;
}

void ServerPacket::SetHello() {
	std::vector<BYTE> hello_packet(packet.size() + 2);

	*(WORD *)&hello_packet[0] = (WORD)packet.size();
	memcpy_s(&hello_packet[2], packet.size(), &packet[0], packet.size());
	packet = hello_packet;
}

void ServerPacket::Encode1(BYTE b) {
	packet.push_back(b);
	encoded++;
}

void ServerPacket::Encode2(WORD w) {
	Encode1(w & 0xFF);
	Encode1((w >> 8) & 0xFF);
}

void ServerPacket::Encode4(DWORD dw) {
	Encode2(dw & 0xFFFF);
	Encode2((dw >> 16) & 0xFFFF);
}

void ServerPacket::Encode8(ULONGLONG ull) {
	Encode4(ull & 0xFFFFFFFF);
	Encode4((ull >> 32) & 0xFFFFFFFF);
}

void ServerPacket::EncodeStr(const char *s) {
	size_t len = strlen(s);

	Encode2((WORD)len);
	for (size_t i = 0; i < len; i++) {
		Encode1((BYTE)s[i]);
	}
}