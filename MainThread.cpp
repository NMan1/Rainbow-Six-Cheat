#include "Cheat.h"
#include "Settings.h"
#include "GUI.h"
#include "Authentication.h"
#include <thread>
#include "xor.h"
#include <playsoundapi.h>
#pragma comment(lib, "winmm.lib")

using namespace g_Settings;
using namespace g_Cheat;

KernelDriver Driver;

void Setup();
void AnyRound();
void Action();
void Prep_Action();
void ToggleRage();
void ToggleLegit();

static auto Toggle = false; int j = 0; //Flag 
static bool DoSil = false;  int k = 0; //Flag
static bool DoNo = false;  
void Keys()
{
	while (true)
	{
		if (g_Settings::bStartCheat)
		{
			if (RoundState == 3)
			{
				if (bSilent && (GetAsyncKeyState(iSilentKey) & 0x8000) && (k == 0))
				{
					DoSil = true;
					if (!Toggle)
					{
						PlaySound("sound2.wav", NULL, SND_SYNC);
					}
					else if (Toggle)
					{
						PlaySound("sound1.wav", NULL, SND_SYNC);
					}
				}
				else if (GetAsyncKeyState(iSilentKey) == 0)
					k = 0;//reset the flag

				if ((GetAsyncKeyState(iNoClip) & 0x8000) && (j == 0))
					DoNo = true;
				else if (GetAsyncKeyState(iNoClip) == 0)
					j = 0; //reset the flag
			}
		}

		Sleep(50);
	}
}

void Cheat::MainCheat()
{
	bool DoOnce = true;
	while (true)
	{
		if (g_Settings::bStartCheat)
		{
			// Open Driver Handle
			if (DoOnce)
			{
				DoOnce = false;
				Sleep(100);
			}

			RoundState = Driver.GetRound();
			if (OldRoundState != RoundState)
			{
				switch (RoundState)
				{
				case 0: Status = "   Round Swap"; break;
				case 1: Status = "   OP Selection"; break;
				case 2: Status = "   Prep Phase"; break;
				case 3: Status = "   Action Phase"; break;
				case 4: Status = "   End Of Round"; break;
				case 5: Status = "   Main Menu"; break;
				}

				if ((OldRoundState == 5 && RoundState == 3) || (OldRoundState == 1 && RoundState == 2))
				{
					//Sleep(1500);
					//Driver.SendMsg(("MNG"));
					Sleep(2000);
					SetSettings(false, true, true, true);
				}
				OldRoundState = RoundState;
			}

			if (DoSil)
			{
				if (!Toggle)
				{
					Driver.SendMsg("SILOF");
					Toggle = true;
				}
				else if (Toggle)
				{
					Driver.SendMsg("SILON");
					Toggle = false;
				}
				k = 1;
				DoSil = false;
			}

			if (DoNo)
			{
				bNoClip = !bNoClip;
				if (bNoClip)
					Driver.SendMsg("CLIP");
				else //false
					Driver.SendMsg("OFCLIP");
				j = 1;
				DoNo = false;
			}

			if (bUpdateSettings)
			{
				Driver.UpdateSettings();
				bUpdateSettings = false;
			}

			if (bUpdate)
			{
				Driver.SendMsg(("MNG"));
				Sleep(2000);
				SetSettings(true, true, true, true);
				bUpdate = false;
			}

			if (bReset)
			{
				Driver.SendMsg(("RESET"));
				bReset = false;
			}

			if (RoundState == 5 || RoundState == 3 || RoundState == 2 || RoundState == 1)
			{
				AnyRound();
			}

			if (RoundState == 2 || RoundState == 3)
			{
				Prep_Action();
			}

			if (RoundState == 3)
			{
				Action();
			}

			// End Whole Cheat
			if (bEnd)
			{
				bCheatThread = true;
				break;
			}

			// End UM App
			if (bEndUM)
				break;

			Sleep(5);
		}
	}

	return;
}

void Setup()
{
	Driver.SendTermAddy();
	Sleep(100);
	Driver.SendMsg((xorstr_("START")));
}

void AnyRound()
{
	if (bUnlockAll && bCanDoUnlockAll)
	{
		Driver.SendMsg(("UNALL"));
		bCanDoUnlockAll = false;
	}

	if (bWepFov && bCanDoWepFov)
	{
		Driver.SendMsg(("FOVWEP"));
		bCanDoWepFov = false;
	}

	if (bPlayerFov && bCanDoPlayerFov)
	{
		Driver.SendMsg("FOVCAR");
		bCanDoPlayerFov = false;
	}
}

void Action()
{
	Cheat::SetSettings(false, true, false, true);
	if (bAimbot)
	{
		while (GetAsyncKeyState(iAimKey) || GetAsyncKeyState(iAimKey2))
		{
			if (bSilent)
				Driver.StartAimbot(true);
			else
				Driver.StartAimbot(false);
		}
	}

	if (bRunShoot && bCanDoRunShoot)
	{
		Driver.SendMsg(("RUNSH"));
		bCanDoRunShoot = false;
	}	

	if (bColor && bCanDoColor)
	{
		Driver.SendMsg(("COLOR"));
		bCanDoColor = false;
	}

	if (bDamage && bCanDoDmg)
	{
		Driver.SendMsg(("DAMAGE"));
		bCanDoDmg = false;
	}

	if (bCavEsp && bCanDoCav)
	{
		Driver.SendCavRequest();
		bCanDoCav = false;
	}
}

void Prep_Action()
{
	if (bChams && bCanDoChams)
	{
		Driver.SendMsg(("CHAMSS"));
		bCanDoChams = false;
	}	

	if (bNoClip && bCanDoNoClip)
	{
		Driver.SendMsg(("CLIP"));
		bCanDoNoClip = false;
	}

	if (bNoFlash && bCanDoNoFlash)
	{
		Driver.SendMsg(("FLASH"));
		bCanDoNoFlash = false;
	}

	if (bSpeed && bCanDoSpeed || (bSpeed && GetAsyncKeyState(VK_SHIFT)))
	{
		Driver.SendMsg(("SPEEDR"));
		bCanDoSpeed = false;
	}

	if (bSpread && bCanDoNS)
	{
		Driver.SendMsg(("SPREAD"));
		bCanDoNS = false;
	}

	if (bNoRecoil && bCanDoNR)
	{
		Driver.SendMsg(("RECOIL"));
		bCanDoNR = false;
	}	

	if (bDamage && bCanDoDmg)
	{
		Driver.SendMsg(("DAMAGE"));
		Driver.SendMsg(("DAMAGE"));
		bCanDoDmg = false;
	}

	if (GetAsyncKeyState(VK_UP))
	{
		Driver.SendMsg(("DAMAGE"));
		Driver.SendMsg(("RECOIL"));
		Driver.SendMsg(("SPREAD"));
	}

	if (bCavEsp && bCanDoCav)
	{
		Sleep(3500);
		Driver.SendCavRequest();
		bCanDoCav = false;
	}
}

void ToggleRage()
{
	// Misc
	fValue = 7;
	iSpeed = 170;
	fSpread = 0;
	iBullet = 10;

	// Aimbot
	iAimFov = 150;
	iAimKey = 2;

	bESP = true;
	bAimbot = true;
	bDamage = true;
	bNoRecoil = true;
	bSpread = true;
	bSpeed = true;
	bNoFlash = true;

	Driver.UpdateSettings();
	Sleep(100);
	Driver.SendMsg(("RAGE"));
	Sleep(1500);
}

void ToggleLegit()
{
	// Misc
	fValue = 7;
	fSpread = 1;

	bAimbot = false;
	bDamage = false;
	bNoRecoil = false;

	Driver.UpdateSettings();
	Sleep(100);
	Driver.SendMsg(("LEGIT"));
	Sleep(500);
}

void Cheat::SetSettings(bool Fov, bool Cav, bool Outline, bool Damage)
{
	/* Set The Settings To True, Allowing The Functions To Run */
	if (Cav)
		bCanDoCav = true;

	if (Outline)
		bCanDoColor = true;

	bCanDoChams = true;
	bCanDoNR = true;
	bCanDoNS = true;
	bCanDoSpeed = true;
	bCanDoDmg = true;
	bCanDoNoFlash = true;
	bCanDoNoClip = true;

	if (Fov)
	{
		bCanDoPlayerFov = true;
		bCanDoWepFov = true;
	}

	return;
}