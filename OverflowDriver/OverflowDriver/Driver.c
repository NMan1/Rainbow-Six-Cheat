// Windows Defines
#include <ntifs.h>
#include <wdm.h>

// User Defined Functions
#include "Features.h"
#include "Helpers.h"
#include "Offsets.h"
#include "Memory.h"
#include "Managers.h"
#include "Clean.h"
#include "Settings.h"
#include "Camera.h"

// Setup
#pragma comment(linker, "/ENTRY:\"RealEntryPoint\"")

// Var Defines
BOOLEAN bReady = FALSE;
LARGE_INTEGER Timeout;
LARGE_INTEGER Timeout2;
DWORD64 LocalPlayer = NULL;
DWORD64 TermProc = NULL;

NTKERNELAPI PVOID PsGetProcessSectionBaseAddress(__in PEPROCESS Process);

NTSTATUS GetImageBase(PEPROCESS Process)
{
	KAPC_STATE State;

	KeStackAttachProcess(Process, &State);
	BaseAddrR6 = (DWORD64*)PsGetProcessSectionBaseAddress(Process);
	KeUnstackDetachProcess(&State);

	DbgPrintEx(0, 0, "Image Found:%p\n", BaseAddrR6);

	return STATUS_SUCCESS;
}

DWORD64 GetModuleBase(PEPROCESS Process, UNICODE_STRING ModuleName)
{
	KAPC_STATE apc;
	if (!Process || !PID_R6)
		return 0;

	memset(&apc, 0, sizeof(apc));
	PROCESS_BASIC_INFORMATION pbi;
	ULONG size = 0;
	HANDLE proc = NULL;
	OBJECT_ATTRIBUTES obj_attr;
	CLIENT_ID cid;

	cid.UniqueProcess = PID_R6; 
	cid.UniqueThread = NULL; 
	InitializeObjectAttributes(&obj_attr, NULL, 0, NULL, NULL);
	ZwOpenProcess(&proc, PROCESS_ALL_ACCESS, &obj_attr, &cid);

	if (!NT_SUCCESS(ZwQueryInformationProcess(proc, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), &size)))
		return 0;

	KeStackAttachProcess(Process, &apc);

	PPEB_LDR_DATA ldr = pbi.PebBaseAddress->Ldr;

	if (!ldr)
	{
		KeUnstackDetachProcess(&apc);
		return 0;
	}

	PVOID found = NULL;
	LIST_ENTRY* head = ldr->InMemoryOrderModuleList.Flink;
	LIST_ENTRY* node = head;
	do 
	{
		PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(node, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		//DbgPrintEx(0, 0, "\nModule Name: '%wZ'", &entry->BaseDllName);

		if (RtlEqualUnicodeString(&entry->BaseDllName, &ModuleName, TRUE))
		{
			DbgPrintEx(0, 0, "\nFound Entry: '%wZ'", &entry->BaseDllName);
			found = entry->DllBase;
			break;
		}

		node = entry->InMemoryOrderLinks.Flink;
	} while (head != node);

	KeUnstackDetachProcess(&apc);
	ZwClose(proc);

	return found;
}

NTSTATUS GetPid()
{
	// ZwQuery
	ULONG CallBackLength = 0;
	PSYSTEM_PROCESS_INFO PSI = NULL;
	PSYSTEM_PROCESS_INFO pCurrent = NULL;
	PVOID BufferPid = NULL;
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	// Names
	PCWSTR R6 = L"RainbowSix.exe";
	UNICODE_STRING uImageNameR6;
	RtlInitUnicodeString(&uImageNameR6, R6);

	if (!NT_SUCCESS(ZwQuerySystemInformation(SystemProcessInformation, NULL, NULL, &CallBackLength)))
	{
		BufferPid = ExAllocatePoolWithTag(NonPagedPool, CallBackLength, 0x616b7963); // aykc 
		if (!BufferPid)
		{
			DbgPrintEx(0, 0, "Failed To Allocate Buffer Notify Routine");
			return Status;
		}
		
		PSI = (PSYSTEM_PROCESS_INFO)BufferPid;
		Status = ZwQuerySystemInformation(SystemProcessInformation, PSI, CallBackLength, NULL);
		if (!NT_SUCCESS(Status))
		{
			DbgPrintEx(0, 0, "Failed To Get Query System Process Information List: %p", Status);
			ExFreePoolWithTag(BufferPid, 0x616b7963);
			return Status = STATUS_INFO_LENGTH_MISMATCH;
		}
		DbgPrintEx(0, 0, "\nSearching For PID...");
		do
		{
			if (PSI->NextEntryOffset == 0)
				break;

			if (RtlEqualUnicodeString(&uImageNameR6, &PSI->ImageName, FALSE))
			{
				DbgPrintEx(0, 0, "PID %d | NAME %ws", PSI->ProcessId, PSI->ImageName.Buffer);
				PID_R6 = PSI->ProcessId;
				Status = STATUS_SUCCESS;
				break;
			}		

			PSI = (PSYSTEM_PROCESS_INFO)((unsigned char*)PSI + PSI->NextEntryOffset); // Calculate the address of the next entry.

		} while (PSI->NextEntryOffset);

		// Free Allocated Memory
		ExFreePoolWithTag(BufferPid, 0x616b7963);
	}

	return Status;
}

NTSTATUS FindGamePID()
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	// Get PID of Rainbow
	Status = GetPid();
	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "Get PID Failed:\n");
		return Status;
	}

	return Status;
}

NTSTATUS GetGameHandle()
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	if (PID_R6)
	{
		Status = PsLookupProcessByProcessId((HANDLE)PID_R6, &R6Process);
		if (!NT_SUCCESS(Status))
		{
			DbgPrintEx(0, 0, "PsLookupProcessByProcessId Failed (Rainbow 6 PID): %p\n", Status);
			return Status;
		}
	}
	else
		return Status;
}

NTSTATUS FindGameBase()
{
	if (R6Process)
		GetImageBase(R6Process);

	return STATUS_SUCCESS;
}

static BOOLEAN bSil = FALSE;
NTSTATUS FindKernelBase()
{
	PCWSTR pKernelBase = L"kernelbase.dll";
	UNICODE_STRING uKernelBase;
	RtlInitUnicodeString(&uKernelBase, pKernelBase);

	KernelBase = GetModuleBase(R6Process, uKernelBase);
	if (!KernelBase)
		return STATUS_UNSUCCESSFUL;

	DbgPrintEx(0, 0, "Kernel Module:%p\n", KernelBase);
	return STATUS_SUCCESS;
}

VOID DriverLoop()
{
	NTSTATUS Status = STATUS_SUCCESS;
	DbgPrintEx(0, 0, "\nMain Driver Loop Started");

	if (!NT_SUCCESS(CreateShareMemory()))
	{
		DbgPrintEx(0, 0, "CreateShareMemory Failed:\n");
		return;
	}	

	while (TRUE)
	{
		/* Begin Checking For Newly Copied Strings */
		Status = ReadSharedMemory();
		if (!NT_SUCCESS(Status))
		{
			DbgPrintEx(0, 0, "Read Shared Memory Failed:%p\n", Status);
			break;
		}

		if (strcmp((PCHAR)g_SharedSection, "Stop") == 0)
		{
			DbgPrintEx(0, 0, "\nMain Loop Ending...\n");
			break;
		}			
		if (strcmp((PCHAR)g_SharedSection, "Find") == 0)
		{
			DbgPrintEx(0, 0, "\nLooking for game...");
			while (TRUE)
			{
				if (FindGamePID() == STATUS_SUCCESS && PID_R6 > 0)
				{
					if (GetGameHandle() == STATUS_SUCCESS)
					{
						if (FindGameBase() == STATUS_SUCCESS)
						{
							pLocalMan = Read(BaseAddrR6 + BASE_OFFSET_PROFILE_MAN, sizeof(DWORD64));
							if (pLocalMan)
							{
								if (GetLocalPlayer())
								{
									DbgPrintEx(0, 0, "Acquired PID and Handle to game...\n");
									const char* Msg = "Found";
									RtlCopyMemory((PCHAR)g_SharedSection, Msg, 9);
									break;
								}
							}
						}
					}
				}
				Timeout.QuadPart = RELATIVE(SECONDS(1));
				KeDelayExecutionThread(KernelMode, FALSE, &Timeout);
			}
		}	

		if (strcmp((PCHAR)g_SharedSection, "START") == 0)
		{
			if (!NT_SUCCESS(FindGameBase()))
			{
				DbgPrintEx(0, 0, "FindGameBase Failed\n");

				/* Clean Up */
				CloseMemory();

				if (R6Process)
					ObDereferenceObject(R6Process);

				break;
			}

			if (!NT_SUCCESS(FindKernelBase()))
				DbgPrintEx(0, 0, "FindKernelBase Failed\n");

			// Managers (calls UpdateResolutions)
			UpdateOffsets();

			// Local Player
			LocalPlayer = GetLocalPlayer();
			if (!LocalPlayer)
				DbgPrintEx(0, 0, "\nLocalPlayer NULL\n");
			else
				DbgPrintEx(0, 0, "\nLocaPlayer:%p\n", LocalPlayer);

			BYTE Ret = 0xC3; BYTE On = 0x0;
			DbgPrintEx(0, 0, "\nTermProc Addy:%p\n", (DWORD64)(TermProc - KernelBase));

			VirtualProtect(KernelBase + (DWORD64)(TermProc - KernelBase), sizeof(BYTE), PAGE_EXECUTE_READWRITE);
			Write(KernelBase + (DWORD64)(TermProc - KernelBase), &Ret, sizeof(Ret));
			VirtualProtect(KernelBase + (DWORD64)(TermProc - KernelBase), sizeof(BYTE), PAGE_EXECUTE_READ);
		}											
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "TERM") == 0)
		{
			// Wait For New Input
			Timeout.QuadPart = RELATIVE(MILLISECONDS(10));
			KeDelayExecutionThread(KernelMode, FALSE, &Timeout);

			// Get User Input
			ReadSharedMemory();
			Kernel_Term* ReadRecived = (Kernel_Term*)g_SharedSection;

			if (!NT_SUCCESS(FindKernelBase()))		
				DbgPrintEx(0, 0, "FindKernelBase Failed\n");
			
			TermProc = ReadRecived->TermAddy;
			break;
		}			
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "MNG") == 0)
		{
			// Managers
			UpdateOffsets();
			break;
		}	
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "SETING") == 0)
		{
			// Wait For New Input
			Timeout.QuadPart = RELATIVE(MILLISECONDS(8));
			KeDelayExecutionThread(KernelMode, FALSE, &Timeout);

			// Get User Input
			ReadSharedMemory();
			Kernel_Settings* ReadRecived = (Kernel_Settings*)g_SharedSection;

			// Initalize
			iSpeed = ReadRecived->iSpeed;
			iBullet = ReadRecived->iBullet;
			fSpread = ReadRecived->fSpread;
			fWepFov = ReadRecived->fWeaponFov;
			fPlayerFov = ReadRecived->fCharFov;
			fChamsR = ReadRecived->ChamsR;
			fChamsG = ReadRecived->ChamsG;
			fChamsB = ReadRecived->ChamsB;
			fChamsOpp = ReadRecived->ChamsOpp;
			fSmooth = ReadRecived->iSmoothing;
			bSilent = ReadRecived->bSilent;
			if (bSilent)
				PatchSilent(TRUE);
			else
				PatchSilent(FALSE);

			iAimFov = ReadRecived->Fov;
			iAimSelection = ReadRecived->AimSelection;
			iBone = ReadRecived->Bone;

			DbgPrintEx(0, 0, "\nSettings Updated!\n");
			break;
		}
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "ROUND") == 0)
		{
			// Wait For New Input
			Timeout.QuadPart = RELATIVE(MILLISECONDS(10));
			KeDelayExecutionThread(KernelMode, FALSE, &Timeout);

			// Get User Input
			ReadSharedMemory();
			Kernel_Get_Round* ReadRecived = (Kernel_Get_Round*)g_SharedSection;
			ReadRecived->Round = GetRound();

			// Send Back Returned Read Address
			ReadSharedMemory();
			if (memcpy(g_SharedSection, ReadRecived, sizeof(Kernel_Get_Round)) == 0)
				DbgPrintEx(0, 0, "Sending Read Address Back Failed\n");

			break;
		}		
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "LIFE") == 0)
		{
			// Wait For New Input
			Timeout.QuadPart = RELATIVE(MILLISECONDS(10));
			KeDelayExecutionThread(KernelMode, FALSE, &Timeout);

			// Get User Input
			ReadSharedMemory();
			Kernel_Get_Life* ReadRecived = (Kernel_Get_Life*)g_SharedSection;
			ReadRecived->Life = GetLifeState();

			// Send Back Returned Read Address
			ReadSharedMemory();
			if (memcpy(g_SharedSection, ReadRecived, sizeof(Kernel_Get_Life)) == 0)
				DbgPrintEx(0, 0, "Sending Read Address Back Failed\n");

			break;
		}				
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "RESET") == 0)
		{
			Speed(LocalPlayer, 100);
			WepFov(0.8726646304);
			CharFov(1.04719758);
			RunChams(7, 0, 40, .4078f, .2980392277, .196078);
			NoRecoil(LocalPlayer, FALSE);
			Spread(LocalPlayer, 1.f);
			BulletMod(1);
		}		
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "LEGIT") == 0)
		{
			Speed(LocalPlayer, iSpeed);
			NoRecoil(LocalPlayer, FALSE);
			Spread(LocalPlayer, fSpread);
			BulletMod(1);
		}		
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "RAGE") == 0)
		{
			LocalPlayer = GetLocalPlayer();
			if (LocalPlayer)
			{ 
				Speed(LocalPlayer, iSpeed);
				NoRecoil(LocalPlayer, TRUE);
				Spread(LocalPlayer, fSpread);
				NoFlash(LocalPlayer);
				BulletMod(10);
				CavRun(LocalPlayer);
			}
		}
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "SILON") == 0)
		{
			PatchSilent(TRUE);
			break;
		}			
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "SILOF") == 0)
		{
			PatchSilent(FALSE);
			break;
		}			
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "AIM") == 0)
		{
			RunAimbot(LocalPlayer, iAimSelection, iBone, iAimFov);
			break;
		}		
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "SIL") == 0)
		{
			RunAimbot(LocalPlayer, iAimSelection, iBone, iAimFov);
			break;
		}				
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "CAVIE") == 0)
		{
			LocalPlayer = GetLocalPlayer();
			if (LocalPlayer)
				CavRun(LocalPlayer);
			break;
		}			
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "COLOR") == 0)
		{
			SetColors();	
			break;
		}			
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "TEAM") == 0)
		{
			SetTeam();	
			break;
		}					
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "CHAMSS") == 0)
		{
			RunChams(3, fChamsOpp, 2.0f, fChamsR, fChamsG, fChamsB);
			break;
		}						
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "FLASH") == 0)
		{
			NoFlash(LocalPlayer);
			break;
		}			
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "DAMAGE") == 0)
		{
			BulletMod(iBullet);
			break;
		}		
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "SPEEDR") == 0)
		{
			Speed(LocalPlayer, iSpeed);
			break;
		}		
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "RECOIL") == 0)
		{
			NoRecoil(LocalPlayer, TRUE);
			break;
		}		
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "SPREAD") == 0)
		{
			Spread(LocalPlayer, fSpread);
			break;
		}			
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "UNALL") == 0)
		{
			UnlockAll();
			break;
		}			
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "FOVCAR") == 0)
		{
			CharFov(fPlayerFov);
			break;
		}			
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "FOVWEP") == 0)
		{
			WepFov(fWepFov);
			break;
		}		
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "CLIP") == 0)
		{
			NoClip(TRUE);
			break;
		}			
		while (!(PCHAR)g_SharedSection == NULL && strcmp((PCHAR)g_SharedSection, "OFCLIP") == 0)
		{
			NoClip(FALSE);
			break;
		}			

		Timeout.QuadPart = RELATIVE(MILLISECONDS(1));
		KeDelayExecutionThread(KernelMode, FALSE, &Timeout);
	}

End:;

	/* Clean Up */
	CloseMemory();

	if (R6Process)
		ObDereferenceObject(R6Process);

	return;
}

NTSTATUS RealEntryPoint()
{
	DbgPrintEx(0, 0, "\nReal Entry Called.");

	// Vars
	NTSTATUS Status = STATUS_SUCCESS;
	OBJECT_ATTRIBUTES ObjAtt = { 0 };
	HANDLE hThread = NULL;
	
	// Clean
	CleanPiDDB();

	// Setup Our Thread
	// TODO: Replace This With Something Like A Hook Idk
	// Not Reliable, Probaly Detected On EAC, Maybe Not BE
	// https://www.unknowncheats.me/forum/anti-cheat-bypass/322618-socket-driver.html
	// https://www.unknowncheats.me/forum/anti-cheat-bypass/325212-eac-system-thread-detection.html
	InitializeObjectAttributes(&ObjAtt, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
	
	Status = PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, &ObjAtt, NULL, NULL, DriverLoop, NULL);
	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "PsCreateSystemThread Failed:\n", Status);
		return Status;
	}

	// Spoof thread start address
	//if (NT_SUCCESS(Status))
	//{
	//	PETHREAD pThread;
	//	NTSTATUS status = PsLookupThreadByThreadId(ThreadIDFromHandle(hThread), &pThread);

	//	// Suspend
	//	//KeSuspendThread(pThread);

	//	if (NT_SUCCESS(Status))
	//	{
	//		CONTEXT ctx = { 0 };
	//		ctx.ContextFlags = CONTEXT_ALL;
	//		Status = PsGetContextThread(pThread, &ctx, KernelMode);
	//		DbgPrint("Status 2: %llx\n", Status);
	//		if (NT_SUCCESS(Status))
	//		{
	//			ctx.Rip = ThreadAddress;
	//			Status = PsSetContextThread(pThread, &ctx, KernelMode);
	//			DbgPrint("[3] Status: %llx\n", Status);
	//		}
	//	}

	//	//KeResumeThread(pThread);

	//	if (pThread)
	//		ObDereferenceObject(pThread);
	//}

	return Status;
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObj, _In_ PUNICODE_STRING RegistryPath)
{
	DbgPrintEx(0, 0, "Driver Created.\n");

	// Fix Paramms
	UNREFERENCED_PARAMETER(RegistryPath);
	UNREFERENCED_PARAMETER(DriverObj);

	return STATUS_SUCCESS;
}


