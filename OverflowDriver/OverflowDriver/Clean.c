#include "Clean.h"
#include "Helpers.h"
#include <ntdef.h>

unsigned __int64 dereference(unsigned __int64 address, unsigned int offset)
{
	if (address == 0)
		return 0;

	return address + (int)((*(int*)(address + offset) + offset) + sizeof(int));
}

NTSTATUS CleanPiDDB()
{
	PCWSTR pDrivNam = L"monitor.sys";
	UNICODE_STRING DrivNam;
	RtlInitUnicodeString(&DrivNam, pDrivNam);

	DbgPrintEx(0, 0, "\nCleanPiDDB started!");
	PRTL_AVL_TABLE PiDDBCacheTable;

	size_t size;
	uintptr_t ntoskrnlBase = GetKenAddress("ntoskrnl.exe", &size);

	//DbgPrintEx(0, 0, "ntoskrnl.exe: %d\n", ntoskrnlBase);
	//DbgPrintEx(0, 0, "ntoskrnl.exe size: %d\n", size);

	PiDDBCacheTable = (PRTL_AVL_TABLE)dereference(PatternScan((void*)ntoskrnlBase, size, "\x48\x8d\x0d\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x3d\x00\x00\x00\x00\x0f\x83", "xxx????x????x????xx"), 3);

	//DbgPrintEx(0, 0, "PiDDBCacheTable: %d\n", PiDDBCacheTable);

	if (!PiDDBCacheTable) 
	{
		DbgPrintEx(0, 0, "PiDDBCacheTable NULL\n");
		return 0;
	}

	uintptr_t entry_address = (uintptr_t)(PiDDBCacheTable->BalancedRoot.RightChild) + sizeof(RTL_BALANCED_LINKS);
	//DbgPrintEx(0, 0, "entry_address: %d\n", entry_address);

	piddbcache* entry = (piddbcache*)(entry_address);

	/*capcom.sys(drvmap) : 0x57CD1415 iqvw64e.sys(kdmapper) : 0x5284EAC3, also cpuz driver*/
	if (entry->TimeDateStamp == 0x57CD1415 || entry->TimeDateStamp == 0x5284EAC3) 
	{
		entry->TimeDateStamp = 0x54EAC3;
		entry->DriverName = DrivNam;
	}

	ULONG count = 0;
	for (PLIST_ENTRY link = entry->List.Flink; link != entry->List.Blink; link = link->Flink, count++)
	{
		piddbcache* cache_entry = (piddbcache*)(link);
		if (cache_entry->TimeDateStamp == 0x57CD1415 || cache_entry->TimeDateStamp == 0x5284EAC3) 
		{
			DbgPrintEx(0, 0, "\nCacheEntry Count: %lu Name: %wZ \t\t Stamp: %x\n",
				count,
				cache_entry->DriverName,
				cache_entry->TimeDateStamp);

			cache_entry->TimeDateStamp = 0x54EAC4 + count;
			cache_entry->DriverName = DrivNam;
			DbgPrintEx(0, 0, "CacheEntry Count: %lu Name: %wZ \t\t Stamp: %x\n",
				count,
				cache_entry->DriverName,
				cache_entry->TimeDateStamp);
		}
	}
}
