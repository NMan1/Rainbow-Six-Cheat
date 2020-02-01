#pragma once
#include "Features.h"
#include "Camera.h"
#include <wdftypes.h>
#include "Settings.h"
#include <math.h>

float CrosshairDistance(float* vScreenPos)
{
	return sqrt(pow(vScreenPos[1] - (iHeight / 2.f), 2.f) + pow(vScreenPos[0] - (iWidth / 2.f), 2.f));
}

UINT64 GetBone(int i)
{
	if (i == 0)
		return 0;
	else if (i == 1)
		return OFFSET_HEAD;
	else if (i == 2)
		return OFFSET_NECK;
	else if (i == 3)
		return OFFSET_CHEST;
	else if (i == 4)
		return OFFSET_STOMACH;
	else if (i == 5)
		return OFFSET_PELVIS;
	else if (i == 6)
		return OFFSET_RIGHT_HAND;
	else if (i == 7)
		return OFFSET_FEET;
}

void GetLocalView(DWORD64 LocalPlayer, float* LocalAngles)
{
	DWORD64 pSkeleton = Read(LocalPlayer + OFFSET_PROFILE_SKELETON, sizeof(DWORD64));
	if (pSkeleton)
	{
		DWORD64 viewAngle2 = Read(pSkeleton + OFFSET_PROFILE_VIEWANGLE[0], sizeof(DWORD64));
		if (!viewAngle2)
			return;

		ReadVector4(viewAngle2 + OFFSET_PROFILE_VIEWANGLE[1], LocalAngles);
	}

	return;
}

BOOLEAN GetClosestBone(DWORD64 LocalPlayer, float* BoneLocation, int iAimFov)
{
	DWORD64 pBestEntity   = NULL;
	float wBestBonePos[3] = { -1, -1, -1 };
	float fBestFov        = iAimFov;
	float fMaxDistance    = 9999.f;

	// First find closest enemy
	for (int i = 0; i < GetMaxEntitys(); i++)
	{
		DWORD64 pEnemy = GetEntityByID(i, LocalPlayer);
		if (!pEnemy)
			continue;

		float fLocation[3] = { -1 }; float vHeadScreen[2];

		GetPlayerHead(fLocation, pEnemy);
		if (fLocation[0] == -1)
			continue;

		if (!WorldToScreen(fLocation, vHeadScreen))
			continue;

		float fDistance = CrosshairDistance(vHeadScreen);
		if (fDistance <= 1)
			return FALSE;

		if (fDistance < fBestFov)
		{
			pBestEntity = pEnemy;
			fBestFov    = fDistance;
		}
	}

	if (!pBestEntity)
	{
		BoneLocation[0] = -1;
		return FALSE;
	}

	for (int i = 1; i < 8; i++)
	{
		float fLocation[3] = { -1 }; float vHeadScreen[2];

		GetPlayerBonePos(GetBone(i), fLocation, pBestEntity);
		if (fLocation[0] == -1)
			continue;

		if (!WorldToScreen(fLocation, vHeadScreen))
			continue;

		float fDistance = CrosshairDistance(vHeadScreen);
		if (fDistance < fMaxDistance)
		{
			fMaxDistance = fDistance;
			wBestBonePos[0] = fLocation[0];
			wBestBonePos[1] = fLocation[1];
			wBestBonePos[2] = fLocation[2];
		}
	}
	BoneLocation[0] = wBestBonePos[0];
	BoneLocation[1] = wBestBonePos[1];
	BoneLocation[2] = wBestBonePos[2];
	return TRUE;
}

BOOLEAN GetClosestEnemy(DWORD64 LocalPlayer, UINT64 iBone, float* BoneLocation, int iAimFov)
{
	float wBoneLocation[3] = { -1, -1, -1 };
	float BestFov = iAimFov; 
	for (int i = 0; i < GetMaxEntitys(); i++)
	{
		DWORD64 Enemy = GetEntityByID(i, LocalPlayer);
		if (!Enemy)
			continue;

		float fLocation[3] = { -1 }; float vHeadScreen[2];
		GetPlayerBonePos(iBone, fLocation, Enemy);

		if (fLocation[0] == -1)
			continue;

		if (!WorldToScreen(fLocation, vHeadScreen))
			continue;

		float distance = CrosshairDistance(vHeadScreen);
		if (distance <= 1)
			return FALSE;

		if (distance < BestFov)
		{
			BestFov = distance;
			wBoneLocation[0] = fLocation[0];
			wBoneLocation[1] = fLocation[1];
			wBoneLocation[2] = fLocation[2];
		}
	}
	BoneLocation[0] = wBoneLocation[0];
	BoneLocation[1] = wBoneLocation[1];
	BoneLocation[2] = wBoneLocation[2];
	return TRUE;
}

#define M_PI		3.14159265358979323846f
static void ToEuler(float* q, float* Out)
{
	float sinr = (float)(+2.0 * (q[3] * q[0] + q[1] * q[2]));
	float cosr = (float)(+1.0 - 2.0 * (q[0] * q[0] + q[1] * q[1]));
	Out[2] = (float)atan2(sinr, cosr);

	double sinp = +2.0 * (q[3] * q[1] - q[2] * q[0]);
	if (abs(sinp) >= 1)
		Out[0] = (float)_copysignf(M_PI / 2, sinp);
	else
		Out[0] = (float)asin(sinp);

	double siny = +2.0 * (q[3] * q[2] + q[0] * q[1]);
	double cosy = +1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]);
	Out[1] = (float)atan2(siny, cosy);
	return;
}

static void VectorAngles(float* forward, float* Out)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[1] == 0)
	{
		yaw = 0;
		if (forward[1] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (float)(atan2(forward[1], forward[0]) * 180 / M_PI);
		tmp = (float)sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (float)(atan2(forward[2] * -1, tmp) * 180 / M_PI);
	}

	yaw += 90;

	if (yaw > 180)
	{
		yaw -= 360;
	}
	if (pitch > 180)
	{
		pitch -= 360;
	}

	Out[0] = pitch;
	Out[1] = yaw;
	Out[2] = 0;
}

static float GetDifference(float firstAngle, float secondAngle)
{
	float difference = secondAngle - firstAngle;
	while (difference < -180.f) difference += 360.f;
	while (difference > 180.f) difference -= 360.f;
	return difference;
}

void RunAimbot(DWORD64 LocalPlayer, int iAimType, int iBone, int iAimFov)
{
	float ViewTranslation[3]; float EnemyViewAngles[3]; float BoneLocation[3];float LocalView[4];

	if (iAimType == 0 && iBone) { if (!GetClosestEnemy(LocalPlayer, GetBone(iBone), BoneLocation, iAimFov)) { return; } }
	else if (iAimType == 0 && iBone == 0) { if (!GetClosestBone(LocalPlayer, BoneLocation, iAimFov)) { return; } }

	if (BoneLocation[0] == -1)
		return;

	GetViewTranslation(ViewTranslation);
	CalcAngle(EnemyViewAngles, BoneLocation, ViewTranslation);
	Clamp(EnemyViewAngles);

	//if (fSmooth > 0)
	//{
	//	float smoothedAngle[3]; float AimAngle[3]; float curAngle[3]; float curAngleV[3];

	//	GetLocalView(LocalPlayer, LocalView);
	//	GetViewTranslation(ViewTranslation);
	//	SubtractVector(AimAngle, ViewTranslation, BoneLocation);
	//	VectorAngles(AimAngle, AimAngle);

	//	ToEuler(LocalView, curAngle);
	//	MultiplyVector(curAngle, (float)(180.f / M_PI));
	//	curAngleV[0] = curAngle[2]; curAngleV[1] = curAngle[1]; curAngleV[0] = curAngle[0];

	//	smoothedAngle[0] = AimAngle[0]; smoothedAngle[1] = AimAngle[1]; smoothedAngle[2] = AimAngle[2];
	//	smoothedAngle[0] = GetDifference(curAngleV[0], smoothedAngle[0]);
	//	smoothedAngle[1] = GetDifference(curAngleV[1], smoothedAngle[1]);
	//	MultiplyVector(smoothedAngle, (fSmooth * .5f));

	//	curAngleV[0] += smoothedAngle[0];
	//	curAngleV[1] += smoothedAngle[1];

	//	MultiplyVector(curAngleV, (float)(M_PI / 180.f));

	//	if (curAngleV[0] > 89.0f && curAngleV[0] <= 180.0f)
	//	{
	//		curAngleV[0] = 89.0f;
	//	}
	//	while (curAngleV[0] > 180.f)
	//	{
	//		curAngleV[0] -= 360.f;
	//	}
	//	while (curAngleV[0] < -89.0f)
	//	{
	//		curAngleV[0] = -89.0f;
	//	}
	//	while (curAngleV[1] > 180.f)
	//	{
	//		curAngleV[1] -= 360.f;
	//	}
	//	while (curAngleV[1] < -180.f)
	//	{
	//		curAngleV[1] += 360.f;
	//	}

	//	float ToAim[4];
	//	CreateFromYawPitchRoll(curAngleV[1], curAngleV[0], 0, ToAim);

	//	DWORD64 pSkeleton = Read(LocalPlayer + OFFSET_PROFILE_SKELETON, sizeof(DWORD64));
	//	if (pSkeleton)
	//	{
	//		DWORD64 viewAngle2 = Read(pSkeleton + OFFSET_PROFILE_VIEWANGLE[0], sizeof(DWORD64));
	//		if (!viewAngle2)
	//			return;

	//		WriteVector4(viewAngle2 + OFFSET_PROFILE_VIEWANGLE[1], ToAim);
	//	}
	//}
	//else
		SetAngle(LocalPlayer, EnemyViewAngles, FALSE);
}
