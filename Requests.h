#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <AclAPI.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <aclapi.h>
#include "Vector.h"
#include "Settings.h"
using namespace std;

typedef struct
{
	// Buffer
	void* Buffer;

	// Misc
	int  iBullet;
	int  iSpeed;
	float fSpread;
	float fWeaponFov;
	float fCharFov;

	// Chams
	float ChamsR;
	float ChamsG;
	float ChamsB;
	float ChamsOpp;

	// Aimbot 
	float    iSmoothing;
	bool bSilent;
	int Fov;
	int AimSelection;
	int Bone;

	// Buffer
	void* Buffer2;

} Kernel_Settings;

typedef struct
{
	DWORD64 Buffer1;
	int Round;
	DWORD64 Buffer2;
} Kernel_Get_Round;

typedef struct
{
	DWORD64 Buffer1;
	int Life;
	DWORD64 Buffer2;
} Kernel_Get_Life;

typedef struct
{
	DWORD64 Buffer1;
	int iAimType;
	int iBone;
	DWORD64 Buffer2;
} Kernel_AimRequest;

typedef struct
{
	DWORD64 Buffer1;
	DWORD64 TermAddy;
	DWORD64 Buffer2;
} Kernel_Term;
 
static bool Do = true;
class KernelDriver
{
public:
	// Global Functions
	inline void CloseHandles()
	{
		CloseHandle(hMapFileR);
		CloseHandle(hMapFileW);		
		CloseHandle(hEspMapFileR);
		CloseHandle(hEspMapFileW);
		return;
	}

	bool OpenFile()
	{
		// Get Handle Read To Map
		hMapFileR = OpenFileMappingA(FILE_MAP_READ, FALSE, "Global\\OverflowRW");
		if (hMapFileR == INVALID_HANDLE_VALUE || !hMapFileR)
			return false;
		
		hMapFileW = OpenFileMappingA(FILE_MAP_WRITE, FALSE, "Global\\OverflowRW");
		if (hMapFileW == INVALID_HANDLE_VALUE || !hMapFileW)
			return false;
				
		return true;
	}

	bool IsGameOpen()
	{
		if (Do)
		{
			auto MapViewMsg = (char*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, 4096);
			if (!MapViewMsg)
			{
				cout << "MapViewMsg Failed. Func - IsGameOpen" << endl;
				return false;
			}

			char Msg[8];
			strcpy_s(Msg, "Find");



			// Copy Memory Over To Map
			RtlCopyMemory(MapViewMsg, Msg, strlen(Msg) + 1);
			Sleep(30);
			UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memory

			Do = false;
		}

		auto MapViewMsg = (char*)MapViewOfFile(hMapFileR, FILE_MAP_READ, 0, 0, 4096);
		if (!MapViewMsg)
		{
			cout << "MapViewMsg Failed. Func - IsGameOpen" << endl;
			return false;
		}

		if (strncmp(MapViewMsg, "Found", 5) == 0)
		{
			UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memory
			return true;
		}
		else
		{
			UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memory
			return false;
		}
	}

	bool SendMsg(const char* Input)
	{
		auto MapViewMsg = (char*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, 4096);
		if (!MapViewMsg)
		{
			cout << "MapViewMsg Failed. Func - Request" << endl;
			return false;
		}
		char Msg[8];
		strcpy_s(Msg, Input);

		// Copy Memory Over To Map
		RtlCopyMemory(MapViewMsg, Msg, strlen(Msg) + 1);
		UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memory
		Sleep(17);
		return true;
	}	

	bool SendTermAddy()
	{
		auto MapViewMsg = (char*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, 4096);
		if (!MapViewMsg)
		{
			cout << "MapViewMsg Failed. Func - Request" << endl;
			return false;
		}
		char Msg[8];
		strcpy_s(Msg, "TERM");

		// Copy Memory Over To Map
		RtlCopyMemory(MapViewMsg, Msg, strlen(Msg) + 1);
		Sleep(6);
		UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memory

		Kernel_Term* MapViewWrite = (Kernel_Term*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, sizeof(Kernel_Term));
		if (!MapViewWrite)
		{
			cout << "Failed To Send Request Struct (Term)" << endl;
			return false;
		}

		Kernel_Term SendRequest;
		SendRequest.TermAddy = (DWORD64)GetProcAddress(GetModuleHandleA("kernelbase.dll"), "TerminateProcess");

		Kernel_Term* pSendRequest = &SendRequest;
		if (memcpy(MapViewWrite, pSendRequest, sizeof(Kernel_Term)) == 0)
			cout << "Failed To Send Request Structure" << endl;

		UnmapViewOfFile(MapViewWrite);

		return true;
	}

	bool UpdateSettings()
	{
		auto MapViewMsg = (char*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, 4096);
		if (!MapViewMsg)
		{
			cout << "MapViewMsg Failed. Func - Request" << endl;
			return false;
		}
		char Msg[8];
		strcpy_s(Msg, "SETING");

		// Copy Memory Over To Map
		RtlCopyMemory(MapViewMsg, Msg, strlen(Msg) + 1);
		Sleep(6);
		UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memory

		Kernel_Settings* MapViewWrite = (Kernel_Settings*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, sizeof(Kernel_Settings));
		if (!MapViewWrite)
		{
			cout << "Failed To Send Request Struct (WriteMemory)" << endl;
			return false;
		}

		Kernel_Settings SendRequest;
		SendRequest.iSpeed = g_Settings::iSpeed;
		SendRequest.iBullet = g_Settings::iBullet;
		SendRequest.fSpread = g_Settings::fSpread;
		SendRequest.fWeaponFov = g_Settings::fWepFov;
		SendRequest.fCharFov = g_Settings::fPlayerFov;

		SendRequest.ChamsR = g_Settings::fChamsR;
		SendRequest.ChamsG = g_Settings::fChamsG;
		SendRequest.ChamsB = g_Settings::fChamsB;
		SendRequest.ChamsOpp = g_Settings::fChamsOpp;

		SendRequest.iSmoothing = g_Settings::iSmoothing;
		SendRequest.bSilent = g_Settings::bSilent;
		SendRequest.Fov = g_Settings::iAimFov;
		SendRequest.AimSelection = g_Settings::iAimSelection;
		SendRequest.Bone = g_Settings::iAimBone;

		Kernel_Settings* pSendRequest = &SendRequest;
		if (memcpy(MapViewWrite, pSendRequest, sizeof(Kernel_Settings)) == 0)
			cout << "Failed To Send Request Structure" << endl;

		UnmapViewOfFile(MapViewWrite);
		cout << "\nUpdated Settings" << endl;

		return true;
	}

	bool StartAimbot(bool Silent)
	{
		auto MapViewMsg = (char*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, 4096);
		if (!MapViewMsg)
		{
			cout << "MapViewMsg Failed. Func - Request" << endl;
			return false;
		}
		char Msg[8];
		if (Silent)
			strcpy_s(Msg, "SIL");
		else
			strcpy_s(Msg, "AIM");

		// Copy Memory Over To Map
		RtlCopyMemory(MapViewMsg, Msg, strlen(Msg) + 1);		
		UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memory
		return true;
	}

	bool SendCavRequest()
	{
		auto MapViewMsg = (char*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, 4096);
		if (!MapViewMsg)
		{
			cout << "MapViewMsg Failed. Func - Request" << endl;
			return false;
		}
		char Msg[8];
		strcpy_s(Msg, "CAVIE");

		// Copy Memory Over To Map
		RtlCopyMemory(MapViewMsg, Msg, strlen(Msg) + 1);
		UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memory
		Sleep(400);

		return true;
	}

	int GetRound()
	{
		auto MapViewMsg = (char*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, 4096);
		if (!MapViewMsg)
		{
			cout << "MapViewMsg Failed. Func - Round" << endl;
			return -1;
		}
		char Msg[8];
		strcpy_s(Msg, "ROUND");
		
		// Copy Memory Over To Map
		RtlCopyMemory(MapViewMsg, Msg, strlen(Msg) + 1);
		Sleep(4);
		UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memorytypedef struct
		
		Kernel_Get_Round* MapViewWrite = (Kernel_Get_Round*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, sizeof(Kernel_Get_Round));
		if (!MapViewWrite)
		{
			cout << "Failed To Send Round Request Struct" << endl;
			return -1;
		}
		
		Kernel_Get_Round SendRequest;
		SendRequest.Round = -1;
		
		Kernel_Get_Round* pSendRequest = &SendRequest;
		if (memcpy(MapViewWrite, pSendRequest, sizeof(Kernel_Get_Round)) == 0)
			cout << "Failed To Send Round Request Structure" << endl;
		
		UnmapViewOfFile(MapViewWrite);
		Sleep(14);

		Kernel_Get_Round* MapViewReadR = (Kernel_Get_Round*)MapViewOfFile(hMapFileR, FILE_MAP_READ, 0, 0, sizeof(Kernel_Get_Round));
		if (!MapViewReadR)
		{ 
			cout << "Failed To Read Returned Round Int" << endl;
			return -1;
		}

		int Round = MapViewReadR->Round;
		UnmapViewOfFile(MapViewReadR);
		return Round;
	}	

	int GetLifeState()
	{
		auto MapViewMsg = (char*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, 4096);
		if (!MapViewMsg)
		{
			cout << "MapViewMsg Failed. Func - Round" << endl;
			return -1;
		}
		char Msg[8];
		strcpy_s(Msg, "LIFE");
		
		// Copy Memory Over To Map
		RtlCopyMemory(MapViewMsg, Msg, strlen(Msg) + 1);
		Sleep(4);
		UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memorytypedef struct
		
		Kernel_Get_Life* MapViewWrite = (Kernel_Get_Life*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, sizeof(Kernel_Get_Life));
		if (!MapViewWrite)
		{
			cout << "Failed To Send Round Request Struct" << endl;
			return -1;
		}
		
		Kernel_Get_Life SendRequest;
		SendRequest.Life = -1;
		
		Kernel_Get_Life* pSendRequest = &SendRequest;
		if (memcpy(MapViewWrite, pSendRequest, sizeof(Kernel_Get_Life)) == 0)
			cout << "Failed To Send Round Request Structure" << endl;
		
		UnmapViewOfFile(MapViewWrite);
		Sleep(14);

		Kernel_Get_Life* MapViewReadR = (Kernel_Get_Life*)MapViewOfFile(hMapFileR, FILE_MAP_READ, 0, 0, sizeof(Kernel_Get_Life));
		if (!MapViewReadR)
		{ 
			cout << "Failed To Read Returned Round Int" << endl;
			return -1;
		}

		int State = MapViewReadR->Life;
		UnmapViewOfFile(MapViewReadR);
		return State;
	}	

	bool End()
	{
		auto MapViewMsg = (char*)MapViewOfFile(hMapFileW, FILE_MAP_WRITE, 0, 0, 4096);
		if (!MapViewMsg)
		{
			cout << "MapViewMsg Failed. Func - End" << endl;
			return false;
		}
		char Msg[8];
		strcpy_s(Msg, "Stop");

		// Copy Memory Over To Map
		RtlCopyMemory(MapViewMsg, Msg, strlen(Msg) + 1);
		Sleep(60);
		UnmapViewOfFile(MapViewMsg); // Unmap our view of file, allowing new allocations in the virtual memory		

		return true;
	}

private:
	HANDLE hMapFileR;
	HANDLE hMapFileW;

	HANDLE hEspMapFileR;
	HANDLE hEspMapFileW;
};
extern KernelDriver Driver;