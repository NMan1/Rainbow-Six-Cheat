//#include "Features.h"
//
//void TpEnemy(DWORD64 Enemy, DWORD64 LocalPlayer)
//{
//	DWORD64 VaultMan1 = Read(LocalPlayer + 0x98, sizeof(DWORD64));
//	DWORD64 VaultMan = Read(VaultMan1 + 0x148, sizeof(DWORD64));
//
//	float EnemyLocation[3]; float LocalFeet[3];
//
//	GetPlayerHead(EnemyLocation, Enemy);
//	GetPlayerFeet(LocalFeet, LocalPlayer);
//
//	// Force Jump
//	int Force = 1; 	int Write3 = 3;
//
//	int nowvault = Read(VaultMan + 0x13F, sizeof(int));
//	int Style = Read(VaultMan + 0x140, sizeof(int));
//
//	WriteInt(VaultMan + 0xA1, Force, sizeof(int));
//
//	WriteInt(VaultMan + 0xA3, Force, sizeof(int));
//	WriteInt(VaultMan + 0xA4, Force, sizeof(int));
//	WriteInt(VaultMan + 0x13F, Force, sizeof(int));
//	WriteInt(VaultMan + 0x140, Write3, sizeof(int));
//
//	WriteInt(VaultMan + 0x140, Write3, sizeof(int));
//
//	WriteVector3(VaultMan + 0x150, EnemyLocation);
//	WriteVector3(VaultMan + 0x160, EnemyLocation);
//	WriteVector3(VaultMan + 0x180, LocalFeet);
//}
//
//void TpCrosshair(DWORD64 LocalPlayer)
//{
//	DWORD64 VaultMan1 = Read(LocalPlayer + 0x98, sizeof(DWORD64));
//	DWORD64 VaultMan = Read(VaultMan1 + 0x148, sizeof(DWORD64));
//
//	float EnemyLocation[3]; float LocalFeet[3];
//
//	//GetPlayerHead(EnemyLocation, Enemy);
//	//GetPlayerFeet(LocalFeet, LocalPlayer);
//
//	// Force Jump
//	int Force = 1; 	int Write3 = 3;
//
//	int nowvault = Read(VaultMan + 0x13F, sizeof(int));
//	int Style = Read(VaultMan + 0x140, sizeof(int));
//
//	WriteInt(VaultMan + 0xA1, Force, sizeof(int));
//
//	WriteInt(VaultMan + 0xA3, Force, sizeof(int));
//	WriteInt(VaultMan + 0xA4, Force, sizeof(int));
//	WriteInt(VaultMan + 0x13F, Force, sizeof(int));
//	WriteInt(VaultMan + 0x140, Write3, sizeof(int));
//
//	WriteInt(VaultMan + 0x140, Write3, sizeof(int));
//
//	WriteVector3(VaultMan + 0x150, EnemyLocation);
//	WriteVector3(VaultMan + 0x160, EnemyLocation);
//	WriteVector3(VaultMan + 0x180, LocalFeet);
//}
//
//void TpFly(DWORD64 Enemy, DWORD64 LocalPlayer)
//{
//	DWORD64 Rap1 = Read(LocalPlayer + 0x28, sizeof(DWORD64));
//	DWORD64 Rap2 = Read(Rap1 + 0x1C8, sizeof(DWORD64));
//	DWORD64 Rap3 = Read(Rap2 + 0x68, sizeof(DWORD64));
//	DWORD64 RapMan = Read(Rap3 + 0xB0, sizeof(DWORD64));
//
//	float Location[3] = { 50, 50, 50 };
//	WriteVector3(RapMan + 0x1A0, Location);
//	WriteVector3(RapMan + 0x1E0, Location);
//	WriteVector3(RapMan + 0x1B0, Location);
//
//	WriteVector3(RapMan + 0x140, Location);
//	WriteVector3(RapMan + 0x150, Location);
//	WriteVector3(RapMan + 0x160, Location);
//
//
//	//ec3 0x1ao startpositoin same as 0x130
//	//	Vec3 0x1eo Hook position
//	//	Vec3 0x1b0 endpos
//
//
//	//	Vec3 0x140 Currentposition(Set as current ? )
//	//	Vec3 0x150 Pos where able to rappel
//	//	Vec3 0x160 where hook will land
//}