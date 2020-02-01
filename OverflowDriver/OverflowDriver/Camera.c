#include "Camera.h"
#include "Features.h"
#include <math.h>
#include "Settings.h"

void CreateFromYawPitchRoll(float yaw, float pitch, float roll, float* NewAngles)
{
	float cy = cos(yaw * 0.5);
	float sy = sin(yaw * 0.5);
	float cr = cos(roll * 0.5);
	float sr = sin(roll * 0.5);
	float cp = cos(pitch * 0.5);
	float sp = sin(pitch * 0.5);

	NewAngles[3] = cy * cr * cp + sy * sr * sp;
	NewAngles[0] = cy * sr * cp - sy * cr * sp;
	NewAngles[1] = cy * cr * sp + sy * sr * cp;
	NewAngles[2] = sy * cr * cp - cy * sr * sp;
}

void GetViewRight(float* Vector3)
{
	ReadVector3(pCamera + OFFSET_VIEWRIGHT, Vector3);
}

void GetViewUp(float* Vector3)
{
	ReadVector3(pCamera + OFFSET_VIEWUP, Vector3);
}

void GetViewForward(float* Vector3)
{
	ReadVector3(pCamera + OFFSET_VIEWFORWARD, Vector3);
}

void GetViewTranslation(float* Vector3)
{
	ReadVector3(pCamera + OFFSET_VIEWTRANSLATION, Vector3);
}

void GetViewFovX(float* Vector3)
{
	ReadView(pCamera + OFFSET_VIEWFOVX, Vector3);
}

void GetViewFovY(float* Vector3)
{
	ReadView(pCamera + OFFSET_VIEWFOVY, Vector3);
}

static BOOLEAN DoOnce = TRUE;
void PatchSilent(BOOLEAN on)
{
	if (on)
	{
		int silentAngle = 0x134;
		if (DoOnce)
		{
			VirtualProtect(BaseAddrR6 + BASE_OFFSET_SILENT + 3, sizeof(int), PAGE_EXECUTE_READWRITE);
			DoOnce = FALSE;
		}
		Write(BaseAddrR6 + BASE_OFFSET_SILENT + 3, &silentAngle, sizeof(int));
		//VirtualProtect(BaseAddrR6 + BASE_OFFSET_SILENT + 3, sizeof(int), PAGE_EXECUTE_READ);
	}
	else
	{
		int silentAngle = 0xc0;
		//VirtualProtect(BaseAddrR6 + BASE_OFFSET_SILENT + 3, sizeof(int), PAGE_EXECUTE_READWRITE);
		Write(BaseAddrR6 + BASE_OFFSET_SILENT + 3, &silentAngle, sizeof(int));
		//VirtualProtect(BaseAddrR6 + BASE_OFFSET_SILENT + 3, sizeof(int), PAGE_EXECUTE_READ);
	}
}

void SetAngle(DWORD64 LocalPlayer, float* Angle, BOOLEAN Smooth)
{
	float NewAngles[4];

	float d2r = 0.01745329251f;
	CreateFromYawPitchRoll(Angle[2] * d2r, 0.f, Angle[0] * d2r, NewAngles);

	DWORD64 pSkeleton = Read(LocalPlayer + OFFSET_PROFILE_SKELETON, sizeof(DWORD64));

	if (pSkeleton)
	{
		DWORD64 viewAngle2 = Read(pSkeleton + OFFSET_PROFILE_VIEWANGLE[0], sizeof(DWORD64));
		if (!viewAngle2)
			return;

		if (!bSilent)
			WriteVector4(viewAngle2 + OFFSET_PROFILE_VIEWANGLE[1], NewAngles);
		else
			WriteVector4(viewAngle2 + OFFSET_PROFILE_SILENTVIEWANGLE, NewAngles);
	}
}