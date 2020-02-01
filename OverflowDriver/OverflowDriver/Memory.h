#pragma once
#include <cstddef>
#include <ntdef.h>
#include <wdftypes.h>

// Shared Memory Functions
NTSTATUS CreateShareMemory();
NTSTATUS ReadSharedMemory();
VOID CloseMemory();
extern PVOID g_SharedSection;

// Memory Control Functions
NTSTATUS Write(DWORD64 SourceAddress, PVOID TargetAddress, SIZE_T Size);
NTSTATUS WriteByte(DWORD64 SourceAddress, unsigned char TargetAddress, SIZE_T Size);
NTSTATUS WriteFloat(DWORD64 SourceAddress, float TargetAddress, SIZE_T Size);
NTSTATUS WriteInt(DWORD64 SourceAddress, int TargetAddress, SIZE_T Size);
NTSTATUS WriteVector3(DWORD64 SourceAddress, float* TargetAddress);
NTSTATUS WriteVector4(DWORD64 SourceAddress, float* TargetAddress);
DWORD64  Read(DWORD64 SourceAddress, SIZE_T Size);
NTSTATUS ReadVector3(DWORD64 SourceAddress, float* TargetVector);
NTSTATUS ReadVector4(DWORD64 SourceAddress, float* TargetVector);
NTSTATUS ReadView(DWORD64 SourceAddress, float* TargetVector);
NTSTATUS VirtualProtect(DWORD64 Address, SIZE_T Size, ULONG NewProtection);
DWORD64  VirtualAlloc(DWORD64 BaseAddress, ULONG AllocType, ULONG Protection, SIZE_T Size);

// Global Memory Functions
int GetRound();
int GetLifeState();

// Player Functions
unsigned short GetMaxEntitys();
DWORD64 GetLocalPlayer();
DWORD64 GetEntityByID(int it, DWORD64 LocalPlayer);
unsigned short GetPlayerTeam(DWORD64 Player);
void GetPlayerBonePos(UINT64 iBone, float* Return, DWORD64 Entity);
void GetPlayerHead(float* Return, DWORD64 Entity);
void GetPlayerFeet(float* Return, DWORD64 Entity);

// Vector and Math
void NormalizeAngle(float* Angle);
void Clamp(float* Angle);
inline void SubtractVector(float* Product, float* Vector1, float* Vector2);
void AddVector(float* Product, float* Vector1, float* Vector2);
void DivideVector(float* Product, float* Vector1, float Amount);
inline void MultiplyVector(float* Vector, float Amount);
float Length(float* Vector);
float VectorDot(float* VectorStart, float* VectorVariable);
BOOLEAN WorldToScreen(float* Pos, float* Screen);
void CalcAngle(float* NewAngle, float* enemypos, float* camerapos);
