#include <math.h>
#include "Memory.h"
#include "Dependencies.h"
#include "Managers.h"
#include "Helpers.h"
#include <ntstatus.h>
#include "Camera.h"
#include <minwindef.h>

#define LOWORD(l) ((WORD)(l))
#define FLT_MAX  3.402823466e+38F;
#define WORDn(x, n) (*((unsigned short *)&(x) + n))
#define LOWORD(x) WORDn(x, 0)

// Globals
LARGE_INTEGER Timeout;

// Shared Memory Vars
const WCHAR g_SharedSectionName[] = L"\\BaseNamedObjects\\OverflowRW";
SECURITY_DESCRIPTOR SecDescriptor;
PVOID	Buffer = NULL;
ULONG	DaclLength;
PACL	Dacl;
HANDLE	g_Section = NULL;
PVOID   g_SharedSection = NULL;

NTSTATUS CreateShareMemory()
{
	DbgPrint("Creating Memory.\n");
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	Status = RtlCreateSecurityDescriptor(&SecDescriptor, SECURITY_DESCRIPTOR_REVISION);
	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "RtlCreateSecurityDescriptor failed: %p\n", Status);
		return Status;
	}

	DaclLength = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) * 3 + RtlLengthSid(SeExports->SeLocalSystemSid) + RtlLengthSid(SeExports->SeAliasAdminsSid) +
		RtlLengthSid(SeExports->SeWorldSid);
	Dacl = ExAllocatePoolWithTag(PagedPool, DaclLength, 'lcaD');

	if (Dacl == NULL)
	{
		return STATUS_INSUFFICIENT_RESOURCES;
		DbgPrintEx(0, 0, "ExAllocatePoolWithTag failed: %p\n", Status);
	}

	Status = RtlCreateAcl(Dacl, DaclLength, ACL_REVISION);

	if (!NT_SUCCESS(Status))
	{
		ExFreePool(Dacl);
		DbgPrintEx(0, 0, "RtlCreateAcl failed: %p\n", Status);
		return Status;
	}

	Status = RtlAddAccessAllowedAce(Dacl, ACL_REVISION, FILE_ALL_ACCESS, SeExports->SeWorldSid);

	if (!NT_SUCCESS(Status))
	{
		ExFreePool(Dacl);
		DbgPrintEx(0, 0, "RtlAddAccessAllowedAce SeWorldSid failed: %p\n", Status);
		return Status;
	}

	Status = RtlAddAccessAllowedAce(Dacl,
		ACL_REVISION,
		FILE_ALL_ACCESS,
		SeExports->SeAliasAdminsSid);

	if (!NT_SUCCESS(Status))
	{
		ExFreePool(Dacl);
		DbgPrintEx(0, 0, "RtlAddAccessAllowedAce SeAliasAdminsSid failed  : %p\n", Status);
		return Status;
	}

	Status = RtlAddAccessAllowedAce(Dacl,
		ACL_REVISION,
		FILE_ALL_ACCESS,
		SeExports->SeLocalSystemSid);

	if (!NT_SUCCESS(Status))
	{
		ExFreePool(Dacl);
		DbgPrintEx(0, 0, "RtlAddAccessAllowedAce SeLocalSystemSid failed  : %p\n", Status);
		return Status;
	}

	Status = RtlSetDaclSecurityDescriptor(&SecDescriptor,
		TRUE,
		Dacl,
		FALSE);

	if (!NT_SUCCESS(Status))
	{
		ExFreePool(Dacl);
		DbgPrintEx(0, 0, "RtlSetDaclSecurityDescriptor failed  : %p\n", Status);
		return Status;
	}

	UNICODE_STRING SectionName = { 0 };
	RtlInitUnicodeString(&SectionName, g_SharedSectionName);

	OBJECT_ATTRIBUTES ObjAttributes = { 0 };
	InitializeObjectAttributes(&ObjAttributes, &SectionName, OBJ_CASE_INSENSITIVE, NULL, &SecDescriptor);

	LARGE_INTEGER lMaxSize = { 0 };
	lMaxSize.HighPart = 0;
	lMaxSize.LowPart = 1044 * 10;

	/* Begin Mapping */
	Status = ZwCreateSection(&g_Section, SECTION_ALL_ACCESS, &ObjAttributes, &lMaxSize, PAGE_READWRITE, SEC_COMMIT, NULL);
	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "Create Section Failed. Status: %p\n", Status);
		return Status;
	}

	//-----------------------------------------------------------------------------	
	//	 ZwMapViewOfSection
	//	-lMaxSize is the ammount of 'Room' the MapViewOfSection will look at
	//	-ViewSize is how much of the 'Room' will be mapped (if 0 then starts at beggining)
	//-----------------------------------------------------------------------------	

	SIZE_T ulViewSize = 0;
	Status = ZwMapViewOfSection(g_Section, NtCurrentProcess(), &g_SharedSection, 0, lMaxSize.LowPart, NULL, &ulViewSize, ViewShare, 0, PAGE_READWRITE | PAGE_NOCACHE);
	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "Map View Section Failed. Status: %p\n", Status);
		ZwClose(g_Section); //Close Handle
		return Status;
	}

	DbgPrintEx(0, 0, "Shared Memory Created.\n\n");
	ExFreePool(Dacl);

	return Status;
}

NTSTATUS ReadSharedMemory()
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	if (!g_Section)
		return;

	if (g_SharedSection)
		ZwUnmapViewOfSection(NtCurrentProcess(), g_SharedSection);

	SIZE_T ulViewSize = 1044 * 10;
	Status = ZwMapViewOfSection(g_Section, NtCurrentProcess(), &g_SharedSection, 0, ulViewSize, NULL, &ulViewSize, ViewShare, 0, PAGE_READWRITE | PAGE_NOCACHE);
	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "Read Shared Memory Failed. %p\n", Status);
		ZwClose(g_Section); //Close Handle
		return;
	}

	return Status;
}

VOID CloseMemory()
{
	// Free Section Memory
	if (g_SharedSection)
		ZwUnmapViewOfSection(NtCurrentProcess(), g_SharedSection);

	// Closing Handle
	if (g_Section)
		ZwClose(g_Section);
}

NTSTATUS Write(DWORD64 SourceAddress, PVOID TargetAddress, SIZE_T Size)
{
	if (!SourceAddress || SourceAddress > 0x7fffffffffff)
		return 0;

	SIZE_T Bytes;
	NTSTATUS Status = STATUS_SUCCESS;

	DbgPrintEx(0, 0, "\nWrite Address:%p\n", SourceAddress);
	DbgPrintEx(0, 0, "Write szAddress:%x\n", Size);

	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), TargetAddress, R6Process, SourceAddress, Size, KernelMode, &Bytes);

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "Bytes Copied:%x\n", Bytes);
		DbgPrintEx(0, 0, "Write Failed:%p\n", Status);
		return Status;
	}

	return Status;
}

NTSTATUS WriteByte(DWORD64 SourceAddress, unsigned char TargetAddress, SIZE_T Size)
{
	if (!SourceAddress || SourceAddress > 0x7fffffffffff)
		return 0;

	SIZE_T Bytes;
	NTSTATUS Status = STATUS_SUCCESS;

	DbgPrintEx(0, 0, "\nWrite Address:%p\n", SourceAddress);
	DbgPrintEx(0, 0, "Write szAddress:%x\n", Size);

	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), &TargetAddress, R6Process, SourceAddress, Size, KernelMode, &Bytes);

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "Bytes Copied:%x\n", Bytes);
		DbgPrintEx(0, 0, "Write Failed:%p\n", Status);
		return Status;
	}

	return Status;
}

NTSTATUS WriteFloat(DWORD64 SourceAddress, float TargetAddress, SIZE_T Size)
{
	if (!SourceAddress || SourceAddress > 0x7fffffffffff)
		return 0;

	SIZE_T Bytes;
	NTSTATUS Status = STATUS_SUCCESS;

	DbgPrintEx(0, 0, "\nWrite Address:%p\n", SourceAddress);
	DbgPrintEx(0, 0, "Write szAddress:%x\n", Size);

	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), &TargetAddress, R6Process, SourceAddress, Size, KernelMode, &Bytes);

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "Write Failed:%p\n", Status);
		return Status;
	}

	return Status;
}

NTSTATUS WriteInt(DWORD64 SourceAddress, int TargetAddress, SIZE_T Size)
{
	if (!SourceAddress || SourceAddress > 0x7fffffffffff)
		return 0;

	SIZE_T Bytes;
	NTSTATUS Status = STATUS_SUCCESS;

	DbgPrintEx(0, 0, "\nWrite Address:%p\n", SourceAddress);
	DbgPrintEx(0, 0, "Write szAddress:%x\n", Size);

	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), &TargetAddress, R6Process, SourceAddress, Size, KernelMode, &Bytes);

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "Write Failed:%p\n", Status);
		return Status;
	}

	return Status;
}

NTSTATUS WriteVector3(DWORD64 SourceAddress, float* TargetAddress)
{
	if (!SourceAddress || SourceAddress > 0x7fffffffffff)
		return 0;

	SIZE_T Bytes;
	NTSTATUS Status = STATUS_SUCCESS;

	DbgPrintEx(0, 0, "\nWrite Vector 3:%p", SourceAddress);
	DbgPrintEx(0, 0, "Write szAddress:%x\n", sizeof(float));

	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), &TargetAddress[0], R6Process, SourceAddress, sizeof(float), KernelMode, &Bytes); // 0x4 
	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), &TargetAddress[1], R6Process, SourceAddress + 0x4, sizeof(float), KernelMode, &Bytes); // 0x8
	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), &TargetAddress[2], R6Process, SourceAddress + 0x8, sizeof(float), KernelMode, &Bytes); // 0x12 add 4 bytes

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "Write Failed:%p\n", Status);
		return Status;
	}

	return Status;
}

NTSTATUS WriteVector4(DWORD64 SourceAddress, float* TargetAddress)
{
	if (!SourceAddress || SourceAddress > 0x7fffffffffff)
		return 0;

	SIZE_T Bytes;
	NTSTATUS Status = STATUS_SUCCESS;

	DbgPrintEx(0, 0, "\nWrite Vector 4:%p", SourceAddress);

	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), &TargetAddress[0], R6Process, SourceAddress, sizeof(float), KernelMode, &Bytes); // 0x4 
	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), &TargetAddress[1], R6Process, SourceAddress + 0x4, sizeof(float), KernelMode, &Bytes); // 0x8
	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), &TargetAddress[2], R6Process, SourceAddress + 0x8, sizeof(float), KernelMode, &Bytes); // 0x12 add 4 bytes
	Status = MmCopyVirtualMemory(PsGetCurrentProcess(), &TargetAddress[3], R6Process, SourceAddress + 0xC, sizeof(float), KernelMode, &Bytes); // 0x12 add 4 bytes

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "WriteVector4 Failed:%p\n", Status);
		return Status;
	}

	return Status;
}

DWORD64 Read(DWORD64 SourceAddress, SIZE_T Size)
{ 
	if (!SourceAddress || SourceAddress > 0x7fffffffffff)
		return 0;

	KAPC_STATE  State;
	NTSTATUS	Status = STATUS_SUCCESS;
	PSIZE_T 	Bytes  = 0;
	void*       TempRead = NULL;

	DbgPrintEx(0, 0, "\nRead Address:%p\n", SourceAddress);
	DbgPrintEx(0, 0, "Read szAddress:%x\n", Size);

	Status = MmCopyVirtualMemory(R6Process, SourceAddress, PsGetCurrentProcess(), &TempRead, Size, KernelMode, &Bytes);

	DbgPrintEx(0, 0, "Read Bytes:%p\n", Bytes);
	DbgPrintEx(0, 0, "Read Output:%p\n", TempRead);

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "Read Failed:%p\n", Status);
		return NULL;
	}

	return TempRead;
} 

NTSTATUS ReadVector3(DWORD64 SourceAddress, float* TargetVector)
{
	if (!SourceAddress || SourceAddress > 0x7fffffffffff)
		return 0;

	PSIZE_T Bytes = 0;
	NTSTATUS Status = STATUS_SUCCESS;

	DbgPrintEx(0, 0, "\nReadVector Address:%p\n", SourceAddress);

	Status = MmCopyVirtualMemory(R6Process, SourceAddress, PsGetCurrentProcess(), &TargetVector[0], sizeof(float), KernelMode, &Bytes);
	Status = MmCopyVirtualMemory(R6Process, (SourceAddress + 0x4), PsGetCurrentProcess(), &TargetVector[1], sizeof(float), KernelMode, &Bytes);
	Status = MmCopyVirtualMemory(R6Process, (SourceAddress + 0x8), PsGetCurrentProcess(), &TargetVector[2], sizeof(float), KernelMode, &Bytes);

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "ReadVector Failed:%p\n", Status);
		return Status;
	}

	return Status;
}

NTSTATUS ReadVector4(DWORD64 SourceAddress, float* TargetVector)
{
	if (!SourceAddress || SourceAddress > 0x7fffffffffff)
		return 0;

	PSIZE_T Bytes = 0;
	NTSTATUS Status = STATUS_SUCCESS;

	DbgPrintEx(0, 0, "\nReadVector Address:%p\n", SourceAddress);

	Status = MmCopyVirtualMemory(R6Process, SourceAddress, PsGetCurrentProcess(), &TargetVector[0], sizeof(float), KernelMode, &Bytes);
	Status = MmCopyVirtualMemory(R6Process, (SourceAddress + 0x4), PsGetCurrentProcess(), &TargetVector[1], sizeof(float), KernelMode, &Bytes);
	Status = MmCopyVirtualMemory(R6Process, (SourceAddress + 0x8), PsGetCurrentProcess(), &TargetVector[2], sizeof(float), KernelMode, &Bytes);
	Status = MmCopyVirtualMemory(R6Process, (SourceAddress + 0x10), PsGetCurrentProcess(), &TargetVector[3], sizeof(float), KernelMode, &Bytes);

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "ReadVector Failed:%p\n", Status);
		return Status;
	}

	return Status;
}

NTSTATUS ReadView(DWORD64 SourceAddress, float* TargetVector)
{
	if (!SourceAddress || SourceAddress > 0x7fffffffffff)
		return 0;

	PSIZE_T Bytes = 0;
	NTSTATUS Status = STATUS_SUCCESS;

	DbgPrintEx(0, 0, "\nReadView Address:%p\n", SourceAddress);

	Status = MmCopyVirtualMemory(R6Process, SourceAddress, PsGetCurrentProcess(), &TargetVector[0], sizeof(float), KernelMode, &Bytes);

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "ReadView Failed:%p\n", Status);
		return Status;
	}

	return Status;
}

DWORD64 VirtualAlloc(DWORD64 BaseAddress, ULONG AllocType, ULONG Protection, SIZE_T Size)
{
	NTSTATUS Status = STATUS_SUCCESS;
	KAPC_STATE apc;
	ULONG old_protection;
	DWORD64 Addy = BaseAddress;

	KeStackAttachProcess(R6Process, &apc);
	Status = ZwAllocateVirtualMemory(ZwCurrentProcess(), &Addy, 0, &Size, AllocType, Protection);
	KeUnstackDetachProcess(&apc);

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "ZwAllocateVirtualMemory Failed:%p\n", Status);
		return Status;
	}

	return Addy;
}

NTSTATUS VirtualProtect(DWORD64 Address, SIZE_T Size, ULONG NewProtection)
{
	NTSTATUS Status = STATUS_SUCCESS;
	KAPC_STATE apc; ULONG OldPro = 0;
	DWORD64* pAddress = Address;

	// Protect Address
	KeStackAttachProcess(R6Process, &apc);
	Status = ZwProtectVirtualMemory(ZwCurrentProcess(), &pAddress, &Size, NewProtection, &OldPro);
	KeUnstackDetachProcess(&apc);

	return Status;
}

int GetRound()
{
	return Read(pRoundMan + OFFSET_ROUND, sizeof(int));
}

int GetLifeState()
{
	DWORD64 Local = GetLocalPlayer();
	DWORD64 a =  Read(Local + 0x28, sizeof(DWORD64));
	DWORD64 b =  Read(a + 0xD8, sizeof(DWORD64));
	DWORD64 c =  Read(b + 0x8, sizeof(DWORD64));
	return Read(c + 0x128, sizeof(int));
}

unsigned short GetMaxEntitys()
{
	unsigned short Entitys = Read(pGameMan + OFFSET_MAX_ENTITYS, sizeof(unsigned short));
	DbgPrintEx(0, 0, "\nEntitys:%hu\n", Entitys);

	return Entitys;
}

unsigned short GetPlayerTeam(DWORD64 Player)
{
	DWORD64 Replication = Read(Player + OFFSET_ENTITY_REPLICATION, sizeof(DWORD64));
	if (!Replication)
		return 0xFF; // Means bot - they dont have entity info structure

	unsigned short OnlineTeamID = Read(Replication + OFFSET_ENTITY_REPLICATION_TEAM, sizeof(unsigned short));

	return (OnlineTeamID);
}

DWORD64 GetLocalPlayer()
{
	DWORD64 EntityContainer = Read(pLocalMan + OFFSET_PROFILE_CONTAINER, sizeof(DWORD64));
	if (!EntityContainer)
		return NULL;

	EntityContainer = Read(EntityContainer, sizeof(void*)); // De Ref
	if (!EntityContainer)
		return NULL;

	return Read(EntityContainer + OFFSET_PROFILE_LOCALENTITY, sizeof(DWORD64));
}

DWORD64 GetEntityByID(int it, DWORD64 LocalPlayer) 
{
	unsigned short LocalTeam = GetPlayerTeam(LocalPlayer);
	if (LocalTeam == 0xFF)
		return NULL;

	DWORD64 Entity = Read(pEntiyList + (sizeof(PVOID) * it), sizeof(DWORD64));
	if (!Entity)
		return NULL;
	
	if (LocalPlayer == Entity)
		return NULL;

	unsigned short EnemyTeam = GetPlayerTeam(Entity);
	if (EnemyTeam == LocalTeam)
		return NULL;

	return Entity;
}

inline void SubtractVector(float* Product, float* Vector1, float* Vector2)
{
	Product[0] = Vector1[0] - Vector2[0]; 
	Product[1] = Vector1[1] - Vector2[1]; 
	Product[2] = Vector1[2] - Vector2[2]; 
}

void AddVector(float* Product, float* Vector1, float* Vector2)
{
	Product[0] = Vector1[0] + Vector2[0]; 
	Product[1] = Vector1[1] + Vector2[1]; 
	Product[2] = Vector1[2] + Vector2[2]; 
}

void DivideVector(float* Product, float* Vector1, float Amount)
{
	Product[0] = Vector1[0] / Amount;
	Product[1] = Vector1[1] / Amount;
	Product[2] = Vector1[2] / Amount;
}

inline void MultiplyVector(float* Vector, float Amount)
{
	for (int i = 0; i < 3; i++)
	{
		Vector[i] = Vector[i] * Amount;
	}
}

float Length(float* Vector)
{
	float ls = Vector[0] * Vector[0] + Vector[1] * Vector[1] + Vector[2] * Vector[2];
	return sqrt(ls);
}

float VectorDot(float* VectorStart, float* VectorVariable)
{
	return VectorStart[0] * VectorVariable[0] + VectorStart[1] * VectorVariable[1] + VectorStart[2] * VectorVariable[2];
}

#define D3DX_PI 3.14159265358979323846
void NormalizeAngle(float* Angle)
{
	while (Angle[0] <= -D3DX_PI)
		Angle[0] += 2 * D3DX_PI;
	while (Angle[0] > D3DX_PI)
		Angle[0] -= 2 * D3DX_PI;
	while (Angle[1] <= -D3DX_PI)
		Angle[1] += 2 * D3DX_PI;
	while (Angle[1] > D3DX_PI)
		Angle[1] -= 2 * D3DX_PI;
}

void Clamp(float* Angle)
{
	if (Angle[0] > 180.f)
		Angle[0] -= 360.f;

	else if (Angle[0] < -180.f)
		Angle[0] += 360.f;

	if (Angle[2] > 180.f)
		Angle[2] -= 360.f;

	else if (Angle[2] < -180.f)
		Angle[2] += 360.f;

	if (Angle[0] < -89.f)
		Angle[0] = -89.f;

	if (Angle[0] > 89.f)
		Angle[0] = 89.f;

	while (Angle[2] < -180.0f)
		Angle[2] += 360.0f;

	while (Angle[2] > 180.0f)
		Angle[2] -= 360.0f;
}

BOOLEAN WorldToScreen(float* Pos, float* Screen)
{
	float Temp[3]; float ViewTranslation[3]; float ViewRight[3]; float ViewUp[3]; float ViewForward[3];
	 
	// Initalize 'Vectors'
	GetViewTranslation(ViewTranslation);
	GetViewRight(ViewRight);
	GetViewUp(ViewUp);
	GetViewForward(ViewForward);

	// Math
	SubtractVector(Temp, Pos, ViewTranslation);
	MultiplyVector(ViewForward, -1.f);

	float x = VectorDot(Temp, ViewRight);
	float y = VectorDot(Temp, ViewUp);
	float z = VectorDot(Temp, ViewForward);

	float ViewX[3]; float ViewY[3];
	GetViewFovX(ViewX); GetViewFovY(ViewY);

	Screen[0] = (iWidth / 2.f) * (1 + x / ViewX[0] / z);
	Screen[1] = (iHeight / 2.f) * (1 - y / ViewY[0] / z);

	return z >= 1.0f ? TRUE : FALSE;
}

void CalcAngle(float* NewAngle, float* enemypos, float* camerapos)
{
	float r2d = 57.2957795131f;
	float dir[3];
	SubtractVector(dir, enemypos, camerapos);

	float x = asin(dir[2] / Length(dir)) * r2d;
	float z = atan(dir[1] / dir[0]) * r2d;

	if (dir[0] >= 0.f) z += 180.f;
	if (x > 180.0f) x -= 360.f;
	else if (x < -180.0f) x += 360.f;

	NewAngle[0] = x;
	NewAngle[1] = 0.f;
	NewAngle[2] = z + 90.f;
}

void GetPlayerBonePos(UINT64 iBone, float* Return, DWORD64 Entity)
{
	if (!iBone)
	{
		Return[0] = -1;
		return;
	}

	DWORD64 pSkeleton = Read(Entity + OFFSET_PROFILE_SKELETON, sizeof(DWORD64));
	if (!pSkeleton)
	{
		Return[0] = -1;
		Return[1] = -1;
		Return[2] = -1;
		return;
	}
	ReadVector3(pSkeleton + iBone, Return);
}

void GetPlayerHead(float* Return, DWORD64 Entity)
{
	DWORD64 pSkeleton = Read(Entity + OFFSET_PROFILE_SKELETON, sizeof(DWORD64));
	if (!pSkeleton)
	{
		Return[0] = -1;
		Return[1] = -1;
		Return[2] = -1;
		return;
	}
	ReadVector3(pSkeleton + OFFSET_HEAD, Return);
}

void GetPlayerFeet(float* Return, DWORD64 Entity)
{
	DWORD64 pSkeleton = Read(Entity + OFFSET_PROFILE_SKELETON, sizeof(DWORD64));
	if (!pSkeleton)
	{
		Return[0] = -1;
		Return[1] = -1;
		Return[2] = -1;
		return;
	}
	ReadVector3(pSkeleton + OFFSET_FEET, Return);
}
