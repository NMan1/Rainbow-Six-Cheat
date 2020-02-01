#pragma once
#include "Dependencies.h"
#include <wdftypes.h>
#include <ntddk.h>
#include <ntstrsafe.h>

// Disable Gay Warnings
#pragma warning( disable : 6387 6066 28023)

#define RELATIVE(wait) (-(wait))

#define NANOSECONDS(nanos) \
(((signed __int64)(nanos)) / 100L)

#define MICROSECONDS(micros) \
(((signed __int64)(micros)) * NANOSECONDS(1000L))

#define MILLISECONDS(milli) \
(((signed __int64)(milli)) * MICROSECONDS(1000L))

#define SECONDS(seconds) \
(((signed __int64)(seconds)) * MILLISECONDS(1000L))

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
	BOOLEAN bSilent;
	int Fov;
	int AimSelection;
	int Bone;

	// Buffer
	void* Buffer2;

} Kernel_Settings;

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
	DWORD64 TermAddy;
	DWORD64 Buffer2;
} Kernel_Term;

typedef struct
{
	DWORD64 Buffer1;
	int Enemys;
	DWORD64 Buffer2;
} Kernel_Get_Enemys;

typedef struct
{
	DWORD64 Buffer1;
	float HeadX;
	float HeadY;
	float FeetX;
	float FeetY;
	DWORD64 Buffer2;
} Kernel_Esp;

typedef struct
{
	float Enemy1[4];
	float Enemy2[4];
	float Enemy3[4];
	float Enemy4[4];
	float Enemy5[4];
} Kernel_Esp2;

typedef enum system_information_class_t
{
	SystemBasicInformation = 0,
	SystemProcessorInformation = 1,             // obsolete...delete
	SystemPerformanceInformation = 2,
	SystemTimeOfDayInformation = 3,
	SystemPathInformation = 4,
	SystemProcessInformation = 5,
	SystemCallCountInformation = 6,
	SystemDeviceInformation = 7,
	SystemProcessorPerformanceInformation = 8,
	SystemFlagsInformation = 9,
	SystemCallTimeInformation = 10,
	SystemModuleInformation = 11,
	SystemLocksInformation = 12,
	SystemStackTraceInformation = 13,
	SystemPagedPoolInformation = 14,
	SystemNonPagedPoolInformation = 15,
	SystemHandleInformation = 16,
	SystemObjectInformation = 17,
	SystemPageFileInformation = 18,
	SystemVdmInstemulInformation = 19,
	SystemVdmBopInformation = 20,
	SystemFileCacheInformation = 21,
	SystemPoolTagInformation = 22,
	SystemInterruptInformation = 23,
	SystemDpcBehaviorInformation = 24,
	SystemFullMemoryInformation = 25,
	SystemLoadGdiDriverInformation = 26,
	SystemUnloadGdiDriverInformation = 27,
	SystemTimeAdjustmentInformation = 28,
	SystemSummaryMemoryInformation = 29,
	SystemMirrorMemoryInformation = 30,
	SystemPerformanceTraceInformation = 31,
	SystemObsolete0 = 32,
	SystemExceptionInformation = 33,
	SystemCrashDumpStateInformation = 34,
	SystemKernelDebuggerInformation = 35,
	SystemContextSwitchInformation = 36,
	SystemRegistryQuotaInformation = 37,
	SystemExtendServiceTableInformation = 38,
	SystemPrioritySeperation = 39,
	SystemVerifierAddDriverInformation = 40,
	SystemVerifierRemoveDriverInformation = 41,
	SystemProcessorIdleInformation = 42,
	SystemLegacyDriverInformation = 43,
	SystemCurrentTimeZoneInformation = 44,
	SystemLookasideInformation = 45,
	SystemTimeSlipNotification = 46,
	SystemSessionCreate = 47,
	SystemSessionDetach = 48,
	SystemSessionInformation = 49,
	SystemRangeStartInformation = 50,
	SystemVerifierInformation = 51,
	SystemVerifierThunkExtend = 52,
	SystemSessionProcessInformation = 53,
	SystemLoadGdiDriverInSystemSpace = 54,
	SystemNumaProcessorMap = 55,
	SystemPrefetcherInformation = 56,
	SystemExtendedProcessInformation = 57,
	SystemRecommendedSharedDataAlignment = 58,
	SystemComPlusPackage = 59,
	SystemNumaAvailableMemory = 60,
	SystemProcessorPowerInformation = 61,
	SystemEmulationBasicInformation = 62,
	SystemEmulationProcessorInformation = 63,
	SystemExtendedHandleInformation = 64,
	SystemLostDelayedWriteInformation = 65,
	SystemBigPoolInformation = 66,
	SystemSessionPoolTagInformation = 67,
	SystemSessionMappedViewInformation = 68,
	SystemHotpatchInformation = 69,
	SystemObjectSecurityMode = 70,
	SystemWatchdogTimerHandler = 71,
	SystemWatchdogTimerInformation = 72,
	SystemLogicalProcessorInformation = 73,
	SystemWow64SharedInformation = 74,
	SystemRegisterFirmwareTableInformationHandler = 75,
	SystemFirmwareTableInformation = 76,
	SystemModuleInformationEx = 77,
	SystemVerifierTriageInformation = 78,
	SystemSuperfetchInformation = 79,
	SystemMemoryListInformation = 80,
	SystemFileCacheInformationEx = 81,
	MaxSystemInfoClass = 82  // MaxSystemInfoClass should always be the last enum

}system_information_class;

static unsigned __int64 PatternScan(void* start, size_t length, const char* pattern, const char* mask)
{
	const char* data = (const char*)start;
	const auto pattern_length = strlen(mask);

	for (size_t i = 0; i <= length - pattern_length; i++)
	{
		BOOLEAN accumulative_found = TRUE;

		for (size_t j = 0; j < pattern_length; j++)
		{
			if (!MmIsAddressValid((void*)((unsigned __int64)data + i + j)))
			{
				accumulative_found = FALSE;
				break;
			}

			if (data[i + j] != pattern[j] && mask[j] != '?')
			{
				accumulative_found = FALSE;
				break;
			}
		}

		if (accumulative_found)
		{
			return (unsigned __int64)((unsigned __int64)data + i);
		}
	}

	return NULL;
}

static uintptr_t GetKenAddress(const char* name, size_t* size)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG neededSize = 0;

	ZwQuerySystemInformation(
		SystemModuleInformation,
		&neededSize,
		0,
		&neededSize
	);

	PSYSTEM_MODULE_INFORMATION pModuleList;

	pModuleList = (PSYSTEM_MODULE_INFORMATION)ExAllocatePool(NonPagedPool, neededSize);

	if (!pModuleList)
	{
		DbgPrintEx(0, 0, "ExAllocatePoolWithTag failed(kernel addr)\n");
		return 0;
	}

	status = ZwQuerySystemInformation(SystemModuleInformation,
		pModuleList,
		neededSize,
		0
	);

	ULONG i = 0;
	uintptr_t address = 0;

	for (i = 0; i < pModuleList->ulModuleCount; i++)
	{
		SYSTEM_MODULE mod = pModuleList->Modules[i];

		address = (uintptr_t)(pModuleList->Modules[i].Base);
		*size = (uintptr_t)(pModuleList->Modules[i].Size);
		if (strstr(mod.ImageName, name) != NULL)
			break;
	}

	ExFreePool(pModuleList);

	return address;
}
