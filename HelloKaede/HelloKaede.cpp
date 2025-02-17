#include"Kyubey.h"
#include"../Share/Simple/Simple.h"

enum MapleRegion {
	REGION_KMS = 1,
	REGION_KMST = 2,
	REGION_JMS = 3,
	REGION_CMS = 4,
	REGION_TWMS = 6,
	REGION_MSEA = 7,
	REGION_GMS = 8,
	REGION_EMS = 9,
	REGION_BMS = 9,
	REGION_VMS = 99,

};

int maple_version = 194;
int maple_version_sub = 0;
MapleRegion maple_region = REGION_JMS;
Alice *ga = NULL;

enum SubControl {
	RESERVED,
	STATIC_VERSION,
	EDIT_VERSION,
	EDIT_HELLO,
	BUTTON_JMS,
	BUTTON_TWMS,
	BUTTON_MSEA,
	BUTTON_EMS,
	BUTTON_KMS,
	BUTTON_KMST,
	BUTTON_CMS,
	BUTTON_GMS,
	BUTTON_BMS,
	BUTTON_THMS,
	BUTTON_VMS,
	BUTTON_IMS,
	TEXTAREA_LOG,
};

void GetHelloPacket(ServerPacket &sp) {
	if (maple_region == REGION_VMS) {
		sp.Encode2(maple_version);
		sp.Encode4(0xFFFFFFFF); // recv
		sp.Encode4(0xFFFFFFFF); // send
		sp.Encode1(maple_region);
		sp.SetHello();
		return;
	}

	if (maple_region != REGION_KMS && maple_region != REGION_KMST) {
		sp.Encode2(maple_version);
		sp.EncodeStr(std::to_string(maple_version_sub).c_str());
	}
	else {
		DWORD xor_version = 0;
		xor_version ^= maple_version;
		xor_version ^= 1 << 15;
		xor_version ^= maple_version_sub << 16;
		sp.Encode2(291);
		sp.EncodeStr(std::to_string(xor_version).c_str());
	}

	sp.Encode4(0xFFFFFFFF); // recv
	sp.Encode4(0xFFFFFFFF); // send
	sp.Encode1(maple_region);
	if (maple_region == REGION_JMS && 327 <= maple_version) {
		sp.Encode1(0);
		sp.Encode1(0);
	}
	sp.SetHello();
}

void HelloServer(Kyubey *client) {
	if (ga) {
		ga->AddText(TEXTAREA_LOG, L"client is connected.");
	}

	ServerPacket sp;
	GetHelloPacket(sp);
	client->SendPacket(sp); // send hello packet
	Sleep(3600 * 100); // wait 1 hours
}

void HelloKaedeThread() {
	Kyubey server(8484, HelloServer);
}

bool OnCreate(Alice &a) {
	ga = &a;
	a.StaticText(STATIC_VERSION, L"Version: ", 30, 30);
	a.EditBox(EDIT_VERSION, 100, 30, L"194.0", 100);

	a.Button(BUTTON_JMS, L"JMS", 250, 10, 60);
	a.Button(BUTTON_KMS, L"KMS", 250, 30, 60);
	a.Button(BUTTON_EMS, L"EMS", 250, 50, 60);
	a.Button(BUTTON_BMS, L"BMS", 250, 70, 60);
	a.Button(BUTTON_TWMS, L"TWMS", 320, 10, 60);
	a.Button(BUTTON_MSEA, L"MSEA", 320, 30, 60);
	a.Button(BUTTON_CMS, L"CMS", 320, 50, 60);
	a.Button(BUTTON_GMS, L"GMS", 320, 70, 60);
	a.Button(BUTTON_VMS, L"VMS", 180, 70, 60);
	a.Button(BUTTON_KMST, L"KMST", 110, 70, 60);

	a.EditBox(EDIT_HELLO, 3, 100, L"HELLO!", 394);
	a.ReadOnly(EDIT_HELLO);
	a.TextArea(TEXTAREA_LOG, 3, 150, 394, 144);
	a.ReadOnly(TEXTAREA_LOG);
	a.SetText(TEXTAREA_LOG, L"ENTER VERSION AND PRESS YOUR CLIENT's REGION");

	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HelloKaedeThread, NULL, NULL, NULL);

	if (hThread) {
		CloseHandle(hThread);
	}
	return true;
}

bool SetServerVersion(Alice &a, MapleRegion mr) {
	std::wstring ver_text = a.GetText(EDIT_VERSION);
	int ver = 0, ver_sub = 0;

	if (swscanf_s(ver_text.c_str(), L"%d.%d", &ver, &ver_sub) != 2) {
		return false;
	}

	maple_version = ver;
	maple_version_sub = ver_sub;
	maple_region = mr;


	ServerPacket sp;
	GetHelloPacket(sp);

	a.SetText(EDIT_HELLO, DatatoString(&sp.get()[0], sp.get().size(), true));
	return true;
}

bool OnCommand(Alice &a, int nIDDlgItem) {

	switch (nIDDlgItem) {
	case BUTTON_JMS:
	{
		SetServerVersion(a, REGION_JMS);
		break;
	}
	case BUTTON_KMS:
	{
		SetServerVersion(a, REGION_KMS);
		break;
	}
	case BUTTON_MSEA:
	{
		SetServerVersion(a, REGION_MSEA);
		break;
	}
	case BUTTON_TWMS:
	{
		SetServerVersion(a, REGION_TWMS);
		break;
	}
	case BUTTON_CMS:
	{
		SetServerVersion(a, REGION_CMS);
		break;
	}
	case BUTTON_GMS:
	{
		SetServerVersion(a, REGION_GMS);
		break;
	}
	case BUTTON_EMS:
	{
		SetServerVersion(a, REGION_EMS);
		break;
	}
	case BUTTON_BMS:
	{
		SetServerVersion(a, REGION_BMS);
		break;
	}
	case BUTTON_VMS:
	{
		SetServerVersion(a, REGION_VMS);
		break;
	}
	case BUTTON_KMST:
	{
		SetServerVersion(a, REGION_KMST);
		break;
	}
	default :{
		break;
	}
	}

	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Alice a(L"HelloKaedeClass", L"HelloKaede v1", 400, 300, hInstance);

	a.SetOnCreate(OnCreate);
	a.SetOnCommand(OnCommand);
	a.Run();
	a.Wait();

	return 0;
}