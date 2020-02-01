#pragma once
#include <basetsd.h>
#include <ntdef.h>

DWORD64 iHeight;
DWORD64 iWidth;

void PatchSilent(BOOLEAN on);
void CreateFromYawPitchRoll(float yaw, float pitch, float roll, float* NewAngles);
void GetViewRight(float* Vector3);
void GetViewUp(float* Vector3);
void GetViewForward(float* Vector3);
void GetViewTranslation(float* Vector3);
void GetViewFovX(float* Vector3);
void GetViewFovY(float* Vector3);
void SetAngle(DWORD64 LocalPlayer, float* Angle, BOOLEAN Smooth);
