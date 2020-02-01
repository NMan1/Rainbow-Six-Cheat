#include "Offsets.h"

/* Cheat Variabls */
ULONG		PID_R6			= 0;
DWORD64		KernelBase		= NULL;
DWORD64		BaseAddrR6		= NULL;
PEPROCESS	R6Process	= NULL;

/* Cheat Values */

// Misc
float fWepFov = 1.4f;
float fPlayerFov = 1.4f;
int iSpeed = 100;
int iBullet = 1;
float fSpread = 1.f;

// Chams
float fChamsR	= 255;
float fChamsG	= 255;
float fChamsB	= 255;
float fChamsOpp = -3;

// Aimbot
float fSmooth = 0;
BOOLEAN bSilent = FALSE;
int iAimFov = 150;
int iAimSelection = 0;
int iBone = 0;

