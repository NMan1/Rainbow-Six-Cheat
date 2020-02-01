#pragma once
#include <string>
#include <d3d9.h>

namespace g_Settings
{
	// Login
	extern bool bStartCheat;
	extern bool bPassFailed;
	extern bool bDisplayLogin;
	extern bool bLoad;
	extern int  iTimeLeft;
	extern std::string sUsername;
	extern std::string sPass;


	// Menu
	extern bool bMenuOpened;
	extern bool bUpdateSettings;

	// Esp
	extern bool bDraw;

	// Visual
	extern bool bESP;
	extern bool bTeam;
	extern bool bOutline;
	extern bool bChams;
	extern bool bCavEsp;

	// Weapon
	extern int  iAimSelection;
	extern int  iAimBone;
	extern int  iAimKey2;
	extern int  iAimKey;
	extern int  iSilentKey;
	extern bool bAimbot;
	extern bool bRunShoot;
	extern bool bDamage;
	extern bool bNoRecoil;
	extern bool bSpread;

	// Misc
	extern bool bUnlockAll;
	extern bool bNoClip;
	extern int iNoClip;
	extern bool bWepFov;
	extern bool bPlayerFov;
	extern bool bSpeed;
	extern bool bNoFlash;

	// Settings
	extern int  iRageKey;
	extern int  iLegitKey;
	extern bool bUpdate;
	extern bool bReset;
	extern bool bEnd;
	extern bool bEndUM;
	extern bool bCheatThread;

	/* VALUES */

	// Misc
	extern float fSpread;
	extern float fWepFov;
	extern float fPlayerFov;
	extern int   iSpeed;
	extern float   fValue;
	extern int  iBullet;
	extern float fBullet;

	// Chams
	extern bool bColor;
	extern float fChamsR;
	extern float fChamsG;
	extern float fChamsB;
	extern float fChamsOpp;
	extern float fOpp;

	// Aimbot
	extern bool bSmoothing;
	extern bool	 bSilent;
	extern int	 iAimFov;
	extern float    iSmoothing;
}

namespace g_Cheat
{
	// Round
	extern bool bDidRoundChange;
	extern int  RoundState;
	extern int  CameraState;
	extern int  OldRoundState;

	// Helpers
	extern int MaxEnemys;

	// Visuals
	extern bool bCanDraw;
	extern bool bCanDoCav;
	extern bool bCanDoNoFlash;
	extern bool bCanDoColor;
	extern bool bCanDoChams;

	// Weapon
	extern bool bCanDoNR;
	extern bool bCanDoRunShoot;
	extern bool bCanDoNS;
	extern bool bCanDoDmg;

	// Misc
	extern bool bCanDoUnlockAll;
	extern bool bCanDoNoClip;
	extern bool bCanDoSpeed;
	extern bool bCanDoWepFov;
	extern bool bCanDoPlayerFov;
}