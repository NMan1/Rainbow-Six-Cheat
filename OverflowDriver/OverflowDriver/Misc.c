#pragma once
#include "Features.h"
#include "Dependencies.h"
#include <minwindef.h>

DWORD64 GetWeaponData(DWORD64 Entity)
{
	DWORD64 WeaponList = Read(Entity + OFFSET_WEAPON[0], sizeof(void*));

	if (!WeaponList)
		return NULL;

	DWORD64 CurrentWeapon = Read(WeaponList + OFFSET_WEAPON[1], sizeof(void*));

	if (!CurrentWeapon)
		return NULL;

	return Read(CurrentWeapon + OFFSET_WEAPON[2], sizeof(void*)); // Current Weapon Data
}

void Spread(DWORD64 Entity, float Value)
{
	DWORD64 CurrentWeaponData = GetWeaponData(Entity);

	if (!CurrentWeaponData)
		return;
	
	WriteFloat(CurrentWeaponData + OFFSET_WEPSPREAD, Value, sizeof(float));
}

void NoRecoil(DWORD64 Entity, BOOLEAN Toggle)
{
	DWORD64 CurrentWeaponData = GetWeaponData(Entity);
	if (!CurrentWeaponData)
		return;

	if (Toggle)
	{
		BYTE On = 0x00;
		VirtualProtect(BaseAddrR6 + BASE_OFFSET_NORECOIL + 2, sizeof(BYTE), PAGE_EXECUTE_READWRITE);
		Write(BaseAddrR6 + BASE_OFFSET_NORECOIL + 2, &On, sizeof(BYTE));
		VirtualProtect(BaseAddrR6 + BASE_OFFSET_NORECOIL + 2, sizeof(BYTE), PAGE_EXECUTE_READ);
	}	
	else if (!Toggle)
	{
		BYTE On = 0x01;
		VirtualProtect(BaseAddrR6 + BASE_OFFSET_NORECOIL + 2, sizeof(BYTE), PAGE_EXECUTE_READWRITE);
		Write(BaseAddrR6 + BASE_OFFSET_NORECOIL + 2, &On, sizeof(BYTE));
		VirtualProtect(BaseAddrR6 + BASE_OFFSET_NORECOIL + 2, sizeof(BYTE), PAGE_EXECUTE_READ);
	}
}

void NoFlash(DWORD64 Entity)
{
	DWORD64 Off1 = Read(Entity + OFFSET_NOFLASH[0], sizeof(DWORD64));
	DWORD64 Flash1 = Read(Off1 + OFFSET_NOFLASH[1], sizeof(DWORD64));
	DWORD64 Flash2 = Read(Flash1 + OFFSET_NOFLASH[2], sizeof(DWORD64));
	int Value = 0;
	Write(Flash2 + OFFSET_NOFLASH[3], &Value, sizeof(UINT8));
}

void UnlockAll()
{
	BYTE On = 0x19;
	BYTE On2 = 0x0;
	DWORD jne_offset = 0x23C;
	DWORD enable_offset = 0x259;

	// Write to unlock all (virtual protect first)
	VirtualProtect(BaseAddrR6 + BASE_OFFSET_UNLOCK_ALL + jne_offset, sizeof(BYTE), PAGE_EXECUTE_READWRITE);
	VirtualProtect(BaseAddrR6 + BASE_OFFSET_UNLOCK_ALL + enable_offset, sizeof(BYTE), PAGE_EXECUTE_READWRITE);

	Write(BaseAddrR6 + BASE_OFFSET_UNLOCK_ALL + jne_offset, &On, sizeof(On));
	Write(BaseAddrR6 + BASE_OFFSET_UNLOCK_ALL + enable_offset, &On2, sizeof(On2));

	VirtualProtect(BaseAddrR6 + BASE_OFFSET_UNLOCK_ALL + jne_offset, sizeof(BYTE), PAGE_EXECUTE_READ);
	VirtualProtect(BaseAddrR6 + BASE_OFFSET_UNLOCK_ALL + enable_offset, sizeof(BYTE), PAGE_EXECUTE_READ);
}

void CharFov(float Value)
{
	DWORD64 Off1 = Read(pFovMan + OFFSET_FOV[0], sizeof(void*));
	DWORD64 Off2 = Read(Off1 + OFFSET_FOV[1], sizeof(void*));
	WriteFloat(Off2 + OFFSET_CHARFOV, Value, sizeof(float));
}

void WepFov(float Value)
{
	DWORD64 Off1 = Read(pFovMan + OFFSET_FOV[0], sizeof(void*));
	DWORD64 Off2 = Read(Off1 + OFFSET_FOV[1], sizeof(void*));
	WriteFloat(Off2 + OFFSET_WEPFOV, Value, sizeof(float));	
}

void Test()
{
	BYTE Shell[] = { 0x49, 0x8b, 0xD0 };
	VirtualProtect(BaseAddrR6 + 0x15ecca8, sizeof(Shell), PAGE_EXECUTE_READWRITE);
	Write(BaseAddrR6 + 0x15ecca8, &Shell, sizeof(Shell));
	VirtualProtect(BaseAddrR6 + 0x15ecca8, sizeof(Shell), PAGE_EXECUTE_READ);
}

void NoClip(BOOLEAN Toggle)
{
	if (Toggle)
	{
		float On[] = { 0, 0, 0 };
		DWORD64 Off1 = Read(pNetMan + OFFSET_NOCLIP[0], sizeof(void*));
		DWORD64 Off2 = Read(Off1 + OFFSET_NOCLIP[1], sizeof(void*));
		WriteVector3(Off2 + OFFSET_NOCLIP[2], On, sizeof(float));
	}
	else
	{
		float Off[] = { 0.0001788139343f, 0.0001788139343f, 0.3051757626f };
		DWORD64 Off1 = Read(pNetMan + OFFSET_NOCLIP[0], sizeof(void*));
		DWORD64 Off2 = Read(Off1 + OFFSET_NOCLIP[1], sizeof(void*));
		WriteVector3(Off2 + OFFSET_NOCLIP[2], Off, sizeof(float));
	}
}

void BulletMod(int Value)
{
	DWORD64 Off2 = Read(pGameMan + OFFSET_BULLET[0], sizeof(void*));
	DWORD64 Off3 = Read(Off2 + OFFSET_BULLET[1], sizeof(void*));
	DWORD64 Off4 = Read(Off3 + OFFSET_BULLET[2], sizeof(void*));
	WriteInt(Off4 + OFFSET_BULLET[3], Value, sizeof(int));
}

void Speed(DWORD64 Entity, int iValue)
{
	DWORD64 LocalSpeed1 = Read(Entity + OFFSET_SPEED[0], sizeof(void*));
	if (!LocalSpeed1)
		return;
	
	DWORD64 LocalSpeed2 = Read(LocalSpeed1 + OFFSET_SPEED[1], sizeof(void*));
	if (!LocalSpeed2)	
		return;
		
	DWORD64 LocalSpeed3 = Read(LocalSpeed2 + OFFSET_SPEED[2], sizeof(void*));
	if (!LocalSpeed2)
		return;
	
	WriteInt(LocalSpeed3 + OFFSET_SPEED[3], iValue, sizeof(int));
}

