#pragma once
#include "Offsets.h"
#include "Memory.h"
#include <ntddk.h>

DWORD64 pNetMan;
DWORD64 pChamsMan;
DWORD64 pGameMan;
DWORD64 pEntiyList;
DWORD64 pStatMan;
DWORD64 pFovMan;
DWORD64 pRoundMan;
DWORD64 pFaceMan;
DWORD64 pLocalMan;
DWORD64 pSetMan;
DWORD64 pCamState;
DWORD64 pCamera;

VOID UpdateOffsets();
VOID UpdateResolutions();
