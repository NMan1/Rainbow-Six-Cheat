#include "Features.h"
#include <wdftypes.h>
#include "Dependencies.h"
#include <windef.h>
#include "Helpers.h"

void SetColors()
{
	float R = 0.0f; float G = 1.f; float B = 0.f; float A = 1.f;
	DWORD64 GMComponentContainer = Read(pGameMan + 0x320, sizeof(DWORD64));
	DWORD64 GMComponentMarkAndOutline = Read(GMComponentContainer + 0x968, sizeof(DWORD64));

	Write(GMComponentMarkAndOutline + 0x48, &R, sizeof(float));
	Write(GMComponentMarkAndOutline + 0x4C, &G, sizeof(float));
	Write(GMComponentMarkAndOutline + 0x50, &B, sizeof(float));
	Write(GMComponentMarkAndOutline + 0x54, &A, sizeof(float));
}

void SetTeam()
{
	int toggle2 = 0;
	DWORD64 GMComponentContainer = Read(pGameMan + 0x320, sizeof(DWORD64));
	DWORD64 GMComponentMarkAndOutline = Read(GMComponentContainer + 0x968, sizeof(DWORD64));
	Write(GMComponentMarkAndOutline + 0x310, &toggle2, sizeof(int));

	BYTE toggle3 = 4;
	DWORD64 local = GetLocalPlayer();
	DWORD64 tmp1 = Read(local + 0xa8, sizeof(DWORD64));
	DWORD64 tmp2 = Read(tmp1 + 0x68, sizeof(DWORD64));
	if (Read(tmp2 + 0x30, sizeof(int) == 3))
		Write(tmp2 + 0x30, &toggle3, sizeof(BYTE));
}

void SetOutline()
{
	BYTE toggle = 1;
	int toggle2 = 0;
	BYTE toggle3 = 4;
	BYTE ArraySize = 5;

	SetColors();
	SetTeam();

	// Increase array size
	DWORD64 GMComponentContainer = Read(pGameMan + 0x320, sizeof(DWORD64));
	DWORD64 GMComponentMarkAndOutline = Read(GMComponentContainer + 0x968, sizeof(DWORD64));
	Write(GMComponentMarkAndOutline + 0x2F8, &ArraySize, sizeof(BYTE));

	for (int i = 0; i < GetMaxEntitys(); i++)
	{
		DWORD64 tmp = Read(pGameMan + 0x1d8, sizeof(DWORD64));
		DWORD64 tmp1 = Read(tmp + i * 0x8, sizeof(DWORD64));
		DWORD64 tmp2 = Read(tmp1 + 0xA70, sizeof(DWORD64));
		DWORD64 tmp3 = Read(tmp2 + 0x30, sizeof(DWORD64));
		DWORD64 tmp4 = Read(tmp3 + 0x48, sizeof(DWORD64));
		Write(tmp4 + 0x110, &toggle, sizeof(BYTE));
	}

	// Freeze array
	Write(GMComponentMarkAndOutline + 0x310, &toggle2, sizeof(BYTE));

	return;
}