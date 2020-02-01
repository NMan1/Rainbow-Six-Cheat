#include "Settings.h"

namespace g_Settings
{
	// Login
	bool bStartCheat		= false;
	bool bPassFailed		= false;
	bool bDisplayLogin		= false;
	bool bLoad				= false;	
	int  iTimeLeft			= -1;
	std::string sUsername;
	std::string sPass;

	// Menu
	bool bMenuOpened		= true;
	bool bUpdateSettings	= true;

	// Esp
	bool bDraw				= false;

	// Visual
	bool bESP				= false;
	bool bOutline			= false;
	bool bTeam  			= false;
	bool bChams				= false;
	bool bCavEsp			= false;

	// Weapon
	int  iAimSelection		= 0;
	int  iAimBone			= 0;
	int  iAimKey2			= 0;
	int  iAimKey			= 0;
	int  iSilentKey = 0;
	bool bAimbot			= false;
	bool bRunShoot			= false;
	bool bDamage			= false;
	bool bNoRecoil			= false;
	bool bSpread			= false;

	// Misc
	bool bUnlockAll			= false;
	int  iNoClip = 0;
	bool bNoClip			= false;
	bool bWepFov			= false;
	bool bPlayerFov			= false;
	bool bSpeed				= false;
	bool bNoFlash			= false;

	// Settings
	int  iRageKey = 0;
	int  iLegitKey = 0;
	
	bool bUpdate			= false;
	bool bReset			    = false;
	bool bEnd				= false;
	bool bEndUM				= false;
	bool bCheatThread		= false;

	/* VALUES */

	// Misc
	float fSpread			= 1.f;
	float fWepFov			= 1.4;
	float fPlayerFov		= 1.4;
	int   iSpeed			= 100;
	float fValue			= 1;
	int  iBullet			= 1;
	float fBullet			= 1;

	// Chams
	bool bColor = false;
	float fChamsR			= 255;
	float fChamsG			= 5;
	float fChamsB			= 5;
	float fChamsOpp			= -3;
	float fOpp = 3;

	// Aimbot
	bool	 bSmoothing = false;
	bool	 bSilent = false;
	int      iAimFov				= 150;
	float    iSmoothing = 0;
}

namespace g_Cheat
{
	// Round
	bool bDidRoundChange	= false;
	int  RoundState			= -1;
	int  CameraState		= -1;
	int  OldRoundState		= -1;

	// Helpers
	int MaxEnemys			= 1;

	// Visuals
	bool bCanDraw			= false;
	bool bCanDoCav			= true;
	bool bCanDoNoFlash		= true;
	bool bCanDoOutline		= true;
	bool bCanDoChams		= true;
	bool bCanDoColor = true;

	// Weapon
	bool bCanDoRunShoot		= true;
	bool bCanDoNR			= true;
	bool bCanDoNS			= true;
	bool bCanDoDmg			= true;

	// Misc
	bool bCanDoUnlockAll	= true;
	bool bCanDoNoClip   	= true;
	bool bCanDoSpeed		= true;
	bool bCanDoWepFov		= true;
	bool bCanDoPlayerFov	= true;

	// Settings
}