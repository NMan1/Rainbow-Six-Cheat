#include "Managers.h"
#include "Camera.h"
#include "Helpers.h"

/*~~~~~~~~~~~~~~~~~~GAME_MAN~~~~~~~~~~~~~~~~~*/
UINT64 BASE_OFFSET_GAME_MAN = 0x5306348; // 48 8B 05 ?? ?? ?? ?? 8B 8E
UINT64 OFFSET_MAX_ENTITYS = 0x1D0;
UINT64 OFFSET_ENTITY_LIST1 = 0x1C8;
UINT64 OFFSET_ENTITY_LIST2 = 0x1D8;
UINT64 OFFSET_ENTITY_REF = 0x28;
UINT64 OFFSET_ENTITY_COUNT = 0x988;
UINT64 OFFSET_ENTITY_REPLICATION = 0xA8;
UINT64 OFFSET_ENTITY_REPLICATION_TEAM = 0x1a8;
/*~~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~RENDER_MAN~~~~~~~~~~~~~~~*/
UINT64 BASE_OFFSET_RENDER_MAN = 0x52f3508;
UINT64 OFFSET_CAMERA[4] = { 0x68, 0x0, 0x130, 0x420 };
UINT64 OFFSET_VIEWFOVX = 0x810;
UINT64 OFFSET_VIEWFOVY = 0x824;
UINT64 OFFSET_VIEWFORWARD = 0x7F0;
UINT64 OFFSET_VIEWUP = 0x7E0;
UINT64 OFFSET_VIEWRIGHT = 0x7D0;
UINT64 OFFSET_VIEWTRANSLATION = 0x800;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~NET_MAN~~~~~~~~~~~~~~~~*/
UINT64 BASE_OFFSET_NET_MAN = 0x52f5bc8; // 48 8b 05 ?? ?? ?? ?? 48 85 c0 0f 84 ?? ?? ?? ?? 48 8b 88 ?? ?? ?? ?? 48 85 c9 0f 84 ?? ?? ?? ?? 4c 8b 8d
UINT64 OFFSET_NOCLIP[3] = { 0xf8, 0x8, 0x530 };
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~CHAM_MAN~~~~~~~~~~~~~~~*/
UINT64 BASE_OFFSET_CHAM_MAN = 0x5f7a4f8; // 48 8b 0d ?? ?? ?? ?? 48 8b d7 e8 ?? ?? ?? ?? 48 85 c0 74 ?? 4c
UINT64 OFFSET_CHAMSMAIN = 0xB8;

UINT64 OFFSET_DIS[2] = { 0x130, 0x134 };
UINT64 OFFSET_OPP  = 0x13C;
UINT64 OFFSET_RGB[3] = { 0x110, 0x114, 0x118 };
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~~FOV_MAN~~~~~~~~~~~~~~~*/
UINT64 BASE_OFFSET_FOV_MAN = 0x5306330; // 48 8b 05 ? ? ? ? f3 44 0f 10 91
UINT64 OFFSET_FOV[2] = { 0x28, 0x0 };
UINT64 OFFSET_CHARFOV = 0x38;
UINT64 OFFSET_WEPFOV= 0x3C;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~ROUND_MAN~~~~~~~~~~~~~~*/
UINT64 BASE_OFFSET_ROUND_MAN = 0x52F5BF8; // 48 8b 0d ?? ?? ?? ?? e8 ?? ?? ?? ?? 83 bb
UINT64 OFFSET_ROUND = 0x2E8;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~PROFILE_MAN~~~~~~~~~~~~*/
UINT64 BASE_OFFSET_PROFILE_MAN = 0x52f5bd8; // 48 8B 05 ?? ?? ?? ?? 33 D2 4C 8B 40 78 
UINT64 OFFSET_PROFILE_CONTAINER = 0x68;
UINT64 OFFSET_PROFILE_LOCALENTITY = 0x28;
UINT64 OFFSET_PROFILE_SKELETON = 0x20;
UINT64 OFFSET_PROFILE_VIEWANGLE[2] = { 0x11A0 , 0xc0 };
UINT64 OFFSET_PROFILE_SILENTVIEWANGLE = 0x134;

UINT64 OFFSET_SPEED[4] = { 0x30, 0x30, 0x38, 0x58 };
UINT64 OFFSET_WEAPON[3] = { 0x78, 0xc8, 0x208 };
UINT64 OFFSET_WEPSPREAD = 0x50;
UINT64 OFFSET_BULLET[4] = { 0x24d8, 0x138, 0x0, 0x40};
UINT64 OFFSET_NOFLASH[4] = { 0x30, 0x30, 0x28, 0x40 };

UINT64 OFFSET_HEAD = 0x670;
UINT64 OFFSET_NECK = 0xF70;
UINT64 OFFSET_RIGHT_HAND = 0x6A0;
UINT64 OFFSET_CHEST = 0xFB0;
UINT64 OFFSET_STOMACH = 0xF90;
UINT64 OFFSET_PELVIS = 0xFD0;
UINT64 OFFSET_FEET = 0x6C0;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~SETTINGS_MAN~~~~~~~~~~~~~*/
UINT64 BASE_OFFSET_SETTINGS_MAN = 0x5f787a8;
UINT64 WIDTH = 0x2e0;
UINT64 HEIGHT = 0x318;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~MARKER_MAN~~~~~~~~~~~~~~~*/
UINT64 BASE_OFFSET_ENTITY_MARKER_VT_OFFSET = 0x395ba68; // 4c 8d 0d ?? ?? ?? ?? 48 ?? ?? ?? 48 8d 8b ?? ?? ?? ?? 4c ?? ?? 48 8d ?? ?? ?? ?? ?? e8
UINT64 OFFSET_ENTITY_COMPONENT = 0x28;
UINT64 OFFSET_ENTITY_COMPONENT_LIST = 0xD8;
UINT64 OFFSET_MAINCOMPONENT_ESPCHAIN = 0x78;
UINT64 OFFSET_ESPCHAIN_WRITE = 0x534;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~*/

UINT64 BASE_OFFSET_UNLOCK_ALL = 0x1138490; // 44 88 44 24 ?? 56 57 41 54 41 55 41 57
UINT64 BASE_OFFSET_NORECOIL   = 0x144F626; // 83 F8 01 0f 85 ?? ?? ?? ?? F3 0F 10 1D
UINT64 BASE_OFFSET_SILENT     = 0x14a46c7; // 0f 10 b3 ? ? ? ? 0f 28 e6 0f 28 de


VOID UpdateResolutions()
{
	//iWidth = Read(pSetMan + WIDTH, sizeof(DWORD64));
	//iHeight = Read(pSetMan + HEIGHT, sizeof(DWORD64));

	iWidth = 2560;
	iHeight = 1440;

	DbgPrintEx(0, 0, "\nScreen Width: %d | Height: %d", iWidth, iHeight);
}

VOID FindOffsets()
{
	size_t Size;
	DWORD64 gBaseAddrR6 = GetKenAddress("RainbowSix.exe", &Size);

	DbgPrintEx(0, 0, "\nRainbowSix.exe: %d | Size: %d", gBaseAddrR6, Size);

	//BASE_OFFSET_NET_MAN = PatternScan()
}

VOID UpdateOffsets()
{
	//FindOffsets();

	pNetMan = Read(BaseAddrR6 + BASE_OFFSET_NET_MAN, sizeof(DWORD64));
	DbgPrintEx(0, 0, "pNetMan:%p\n", pNetMan);

	pChamsMan = Read(BaseAddrR6 + BASE_OFFSET_CHAM_MAN, sizeof(DWORD64));
	DbgPrintEx(0, 0, "pChamsMan:%p\n", pChamsMan);

	pGameMan = Read(BaseAddrR6 + BASE_OFFSET_GAME_MAN, sizeof(DWORD64));
	DbgPrintEx(0, 0, "pGameMan:%p\n", pGameMan);

	pEntiyList = Read(pGameMan + OFFSET_ENTITY_LIST1, sizeof(DWORD64));
	DbgPrintEx(0, 0, "pEntiyLis:%p\n", pEntiyList);

	pFovMan = Read(BaseAddrR6 + BASE_OFFSET_FOV_MAN, sizeof(DWORD64));
	DbgPrintEx(0, 0, "pFovMan:%p\n", pFovMan);

	pRoundMan = Read(BaseAddrR6 + BASE_OFFSET_ROUND_MAN, sizeof(DWORD64));
	DbgPrintEx(0, 0, "pRoundMan:%p\n", pRoundMan);	

	pLocalMan = Read(BaseAddrR6 + BASE_OFFSET_PROFILE_MAN, sizeof(DWORD64));
	DbgPrintEx(0, 0, "pLocalMan:%p\n", pLocalMan);	

	pSetMan = Read(BaseAddrR6 + BASE_OFFSET_SETTINGS_MAN, sizeof(DWORD64));
	DbgPrintEx(0, 0, "pSetMan:%p\n", pSetMan);

	pCamera = Read(BaseAddrR6 + BASE_OFFSET_PROFILE_MAN, sizeof(DWORD64));
	if (!pCamera)
		return;
	pCamera = Read(pCamera + OFFSET_CAMERA[0], sizeof(DWORD64));
	if (!pCamera)
		return;
	pCamera = Read(pCamera + OFFSET_CAMERA[1], sizeof(DWORD64));
	if (!pCamera)
		return;
	pCamera = Read(pCamera + OFFSET_CAMERA[2], sizeof(DWORD64));
	if (!pCamera)
		return;
	pCamera = Read(pCamera + OFFSET_CAMERA[3], sizeof(DWORD64));
	DbgPrintEx(0, 0, "pCamera:%p\n", pCamera);

	UpdateResolutions();
}
