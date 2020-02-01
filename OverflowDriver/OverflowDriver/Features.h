#pragma once
#include "Offsets.h"
#include "Memory.h"
#include "Managers.h"

// Misc
void Speed(DWORD64 Entity, int fValue);
void Spread(DWORD64 Entity, float Value);
void NoRecoil(DWORD64 Entity, BOOLEAN Toggle);
void NoFlash(DWORD64 Entity);
void UnlockAll();
void WepFov(float Value);
void CharFov(float Value);
void BulletMod(int Value);
void NoClip(BOOLEAN Toggle);

// Cav
BOOLEAN CavRun(DWORD64 LocalPlayer);

// Chams
void RunChams(float fOpacity, float EnemyBrightness, float LocalBrightness, float red, float green, float blue);

// Outline
void SetTeam();
void SetColors();
void SetOutline();

// Aimbot
void RunAimbot(DWORD64 LocalPlayer, int iAimType, int iBone, int iAimFov);

