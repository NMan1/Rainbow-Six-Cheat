#include "Features.h"

DWORD64 ChamsBase()
{
	DWORD64 Chams = Read(pChamsMan + OFFSET_CHAMSMAIN, sizeof(void*));
	return Chams;
}

void Rgb(DWORD64 ChamsMan, float red, float green, float blue)
{
	WriteFloat(ChamsMan + OFFSET_RGB[0], red, sizeof(float));
	WriteFloat(ChamsMan + OFFSET_RGB[1], green, sizeof(float));
	WriteFloat(ChamsMan + OFFSET_RGB[2], blue, sizeof(float));
}

void Opacity(DWORD64 ChamsMan, float Value)
{
	WriteFloat(ChamsMan + OFFSET_OPP, Value, sizeof(float));
}

void DistanceOne(DWORD64 ChamsMan, float Value)
{
	WriteFloat(ChamsMan + OFFSET_DIS[0], Value, sizeof(float));
}

void DistanceTwo(DWORD64 ChamsMan, float Value)
{
	WriteFloat(ChamsMan + OFFSET_DIS[1], Value, sizeof(float));
}

void RunChams(float fOpacity, float EnemyBrightness, float LocalBrightness, float red, float green, float blue)
{
	DWORD64 ChamsMan = ChamsBase();
	DistanceTwo(ChamsMan, LocalBrightness);
	Opacity(ChamsMan, fOpacity);
	Rgb(ChamsMan, red, green, blue);
	DistanceOne(ChamsMan, EnemyBrightness);
}

