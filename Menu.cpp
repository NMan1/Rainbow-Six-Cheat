#include "GUI.h"
#include "Settings.h"
#include <Lmcons.h>
#include <time.h>
#include "Requests.h"
#include "xor.h"
#include "Config.h"

// Settings
using namespace g_Settings; 

// Pages
static int iPage = 0;
bool Close = false;

const char* SelectionType[] = { ("Closest FOV") };
const char* BoneType[] = { ("Closest Bone"), ("Head"), ("Neck"), ("Chest"), ("Stomach"), ("Pelvis"), ("Hand"), ("Feet") };
const char* ConfigList[] = { "Default", "Legit", "Rage"};
int iCurrentConfig;

char pConfigName[12]{  };
static auto flags2 = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

Config* DefualtCfg;
Config* LegitCfg;
Config* RageCfg;

void ResetSettings()
{
	// Visual
	bESP = false;
	bOutline = false;
	bTeam = false;
	bChams = false;
	bCavEsp = false;

	// Weapon
	bAimbot = false;
	bRunShoot = false;
	bDamage = false;
	bNoRecoil = false;
	bSpread = false;
	bSilent = false;

	// Misc
	bUnlockAll = false;
	bNoClip = false;
	bWepFov = false;
	bPlayerFov = false;
	bSpeed = false;
	bNoFlash = false;

	/* VALUES */

	// Misc
	fWepFov = .8f;
	fPlayerFov = 1.f;
	iSpeed = 100;
	fSpread = 1.f;
	iBullet = 1;
	iSmoothing = 0;

	// Chams
	fChamsR = 255;
	fChamsG = 255;
	fChamsB = 255;
	fChamsOpp = 3;

	// Aimbot
	iAimFov = 150;
}

void Menu::RenderButtons()
{
	if (iPage == 0)
	{
		PushStyleColor(ImGuiCol_Button, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonHovered, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonActive, ActiveButton);
		if (Button(xorstr_("Visuals"), ImVec2(85, 0)))
			iPage = 0;
		PopStyleColor();
		PopStyleColor();
		PopStyleColor();
	}
	else
	{
		if (Button(xorstr_("Visuals"), ImVec2(85, 0)))
			iPage = 0;
	}	
	ImGui::SameLine();
	if (iPage == 1)
	{
		PushStyleColor(ImGuiCol_Button, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonHovered, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonActive, ActiveButton);		
		if (Button(xorstr_("Weapon"), ImVec2(85, 0)))
			iPage = 1;
		PopStyleColor();
		PopStyleColor();
		PopStyleColor();
	}
	else
	{
		if (Button(xorstr_("Weapon"), ImVec2(85, 0)))
			iPage = 1;
	}	
	ImGui::SameLine();
	if (iPage == 2)
	{
		PushStyleColor(ImGuiCol_Button, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonHovered, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonActive, ActiveButton);		
		if (Button(xorstr_("Misc"), ImVec2(85, 0)))
			iPage = 2;
		PopStyleColor();
		PopStyleColor();
		PopStyleColor();
	}
	else
	{
		if (Button(xorstr_("Misc"), ImVec2(85, 0)))
			iPage = 2;
	}	
	ImGui::SameLine();
	if (iPage == 3)
	{
		PushStyleColor(ImGuiCol_Button, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonHovered, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonActive, ActiveButton);		
		if (Button(xorstr_("Settings"), ImVec2(85, 0)))
			iPage = 3;
		PopStyleColor();
		PopStyleColor();
		PopStyleColor();
	}
	else
	{
		if (Button(xorstr_("Settings"), ImVec2(85, 0)))
			iPage = 3;
	}
}

void Bar()
{
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
}

void SameLineDummy(int xSpacing)
{
	ImGui::SameLine(); Dummy(ImVec2(xSpacing, 0)); ImGui::SameLine();
}

void Menu::RenderMenu()
{
	static bool DoOnce = true;
	if (DoOnce)
	{
		DefualtCfg = new Config("Defualt.cfg");
		LegitCfg = new Config("Legit.cfg");
		RageCfg = new Config("Rage.cfg");

		SetConfig(DefualtCfg);

		DoOnce = false;
	}

	// Style
	auto& style = GetStyle();

	// Menu
	SetNextWindowPos(ImVec2(0, 0));
	SetNextWindowSize(ImVec2(396, 411));
	static const auto dwFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;
	Begin(xorstr_("Overflow"), 0, dwFlags);
	{
		// Multi Colored Rect
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImColor c = ImColor(32, 114, 247);
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 22), ImVec2(p.x + ImGui::GetWindowWidth() - 16, p.y), ImColor(30, 30, 39));
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 22), ImVec2(p.x + ImGui::GetWindowWidth() - 16, p.y + 24), ImColor(234, 57, 57, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));
		ImGui::Spacing();

		// Tilte
		PushFont(Font18);
		ImGui::Text(xorstr_("Overflow    -"));
		ImGui::SameLine();
		PushStyleColor(ImGuiCol_Text, TabTitleText);
		ImGui::Text(Status.c_str());
		PopStyleColor();
		ImGui::PopFont();

		// Main
		ImGui::SetCursorPosX(8);
		ImGui::SetCursorPosY(32);
		BeginChild(xorstr_("Main"), ImVec2(380, 0), true);
		{
			ImGui::PushFont(Font18);
			RenderButtons();

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing(); ImGui::Spacing();

			if (iPage == 0)
			{
				ImGui::Spacing();

				Checkbox(xorstr_("Caveria Esp        "), &bCavEsp);
				SameLineDummy(60);
				Checkbox(xorstr_("Player Chams"), &bChams);
				ImGui::Spacing();
				Checkbox(xorstr_("Outline Color      "), &bColor);
				SameLineDummy(60);
				Checkbox(xorstr_("No Flash"), &bNoFlash);

				Bar();

				ImGui::Text(xorstr_("Chams Red"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(xorstr_("##  #  "), &g_Settings::fChamsR, 0, 255, xorstr_("%.0f")))
				{
					g_Cheat::bCanDoChams = true;
					g_Settings::bUpdateSettings = true;
				}
				ImGui::Spacing(); ImGui::Spacing(); 
				ImGui::Text(xorstr_("Chams Green"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(xorstr_("##  "), &g_Settings::fChamsG, 0, 255, xorstr_("%.0f")))
				{
					g_Cheat::bCanDoChams = true;
					g_Settings::bUpdateSettings = true;
				}
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text(xorstr_("Chams Blue"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(xorstr_("####  "), &g_Settings::fChamsB, 0, 255, xorstr_("%.0f")))
				{
					g_Cheat::bCanDoChams = true;
					g_Settings::bUpdateSettings = true;
				}	
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text(xorstr_("Chams Brightness"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(xorstr_("########    "), &fOpp, 0, 5, xorstr_("%.0f")))
				{
					switch ((int)fOpp)
					{
					case 0:
						g_Settings::fChamsOpp = 0; break;					
					case 1:
						g_Settings::fChamsOpp = -1; break;
					case 2:
						g_Settings::fChamsOpp = -2; break;
					case 3:
						g_Settings::fChamsOpp = -3; break;
					case 4:
						g_Settings::fChamsOpp = -4; break;					
					case 5:
						g_Settings::fChamsOpp = -5; break;
					}

					g_Cheat::bCanDoChams = true;
					g_Settings::bUpdateSettings = true;
				}
			}			
			else if (iPage == 1)
			{
				ImGui::Spacing();

				Checkbox(xorstr_("Enable Aimbot"), &bAimbot);
				SameLineDummy(60);
				Checkbox(xorstr_("Bullet multiplier"), &bDamage);
				ImGui::Spacing();
				Checkbox(xorstr_("No Recoil       "), &bNoRecoil);
				SameLineDummy(60);
				Checkbox(xorstr_("Spread Control"), &bSpread);				

				Bar();

				bool bSil = bSilent;
				Checkbox(xorstr_("Silent Aim      "), &bSilent);
				if (bSil != bSilent)
					g_Settings::bUpdateSettings = true;

				SameLineDummy(61);
				Checkbox(xorstr_("Aim Smoothing"), &bSmoothing);

				Bar();

				ImGui::PushItemWidth(144);
				ImGui::Text(xorstr_("Aimbot FOV                     Aimbot Smooth"));
				ImGui::SetCursorPosX(25);
				if (SliderInt(xorstr_("### "), &iAimFov, 1, 500))
					g_Settings::bUpdateSettings = true;

				ImGui::SameLine();

				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(195 + 16);
				if (SliderFloat(xorstr_("#####  "), &iSmoothing, 0, 10, xorstr_("%.1f")))
					g_Settings::bUpdateSettings = true;

				ImGui::Spacing(); ImGui::Spacing();

				ImGui::PushItemWidth(144);
				ImGui::Text(xorstr_("Bullet Ammount                Spread Amount"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(xorstr_("######     "), &fBullet, 1, 20, xorstr_("%.0f")))
				{
					iBullet = (int)fBullet;
					g_Settings::bUpdateSettings = true;
				}

				ImGui::SameLine();

				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(195 + 16);
				if (SliderFloat(xorstr_("####   "), &g_Settings::fSpread, 0, 5, xorstr_("%.0f")))
				{
					g_Cheat::bCanDoNS = true;
					g_Settings::bUpdateSettings = true;
				}

				ImGui::Spacing(); ImGui::Spacing();

				ImGui::PushItemWidth(144);
				ImGui::Text(xorstr_("Aimbot Selection                Aimbot Bone"));
				ImGui::SetCursorPosX(25);
				if (Combo(xorstr_("##      "), &g_Settings::iAimSelection, SelectionType, IM_ARRAYSIZE(SelectionType)))
					g_Settings::bUpdateSettings = true;

				//Spacing(); Spacing(); 
				ImGui::SameLine();

				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(195+16);
				if (Combo(xorstr_("####      "), &g_Settings::iAimBone, BoneType, IM_ARRAYSIZE(BoneType)))
					g_Settings::bUpdateSettings = true;
			}
			else if (iPage == 2)
			{
				ImGui::Spacing();
				Checkbox(xorstr_("Unlock All"), &bUnlockAll);
				SameLineDummy(10);
				Checkbox(xorstr_("No-Clip"), &bNoClip);
				SameLineDummy(10);
				Checkbox(xorstr_("Speed Changer"), &bSpeed);
				ImGui::Spacing();
				Checkbox(xorstr_("WeaponFov Changer"), &bWepFov);
				SameLineDummy(40);
				Checkbox(xorstr_("Player Fov "), &bPlayerFov);

				Bar();

				ImGui::Text(xorstr_("Speed"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(xorstr_("##   # "), &fValue, 1, 10, xorstr_("%.0f")))
				{
					switch ((int)fValue)
					{
					case 1:
						g_Settings::iSpeed = 110; break;
					case 2:
						g_Settings::iSpeed = 120; break;
					case 3:
						g_Settings::iSpeed = 130; break;
					case 4:
						g_Settings::iSpeed = 140; break;
					case 5:
						g_Settings::iSpeed = 150; break;
					case 6:
						g_Settings::iSpeed = 160; break;
					case 7:
						g_Settings::iSpeed = 170; break;
					case 8:
						g_Settings::iSpeed = 180; break;
					case 9:
						g_Settings::iSpeed = 190; break;
					case 10:
						g_Settings::iSpeed = 200; break;

					}
					g_Cheat::bCanDoSpeed = true;
					g_Settings::bUpdateSettings = true;
				}
				ImGui::Spacing(); ImGui::Spacing();  ImGui::Spacing();
				ImGui::Text(xorstr_("Weapon FOV"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(xorstr_("##  "), &g_Settings::fWepFov, .1, 3, xorstr_("%.1f")))
				{
					g_Cheat::bCanDoWepFov = true;
					g_Settings::bUpdateSettings = true;
				}
				ImGui::Spacing(); ImGui::Spacing();  ImGui::Spacing();
				ImGui::Text(xorstr_("Player FOV"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(xorstr_("####  "), &g_Settings::fPlayerFov, .1, 3, xorstr_("%.1f")))
				{
					g_Cheat::bCanDoPlayerFov = true;
					g_Settings::bUpdateSettings = true;
				}
			}
			else if (iPage == 3)
			{
				ImGui::Spacing();

				PushFont(Font18);
				ImGui::Text(xorstr_("Aimbot Key:                         Backup Key:"));
				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(25);
				HotKey(xorstr_(" "), &iAimKey, Font16);

				SameLineDummy(25);
				HotKey(xorstr_("## "), &iAimKey2, Font16);

				Spacing();

				ImGui::Text(xorstr_("No-Clip Toggle Key:"));
				ImGui::SetCursorPosX(25);
				HotKey(xorstr_("###### "), &iNoClip, Font16);

				Bar();

				ImGui::Text(xorstr_("Configs                                   Toggle Silent:"));
				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(25);
				if (Combo(xorstr_("###    "), &iCurrentConfig, ConfigList, IM_ARRAYSIZE(ConfigList)))
					g_Settings::bUpdateSettings = true;

				SameLineDummy(25);
				HotKey(xorstr_("  "), &iSilentKey, Font16);

				ImGui::Spacing();

				if (Button(xorstr_("Load Config"), ImVec2(178, 0)))
				{
					switch (iCurrentConfig)
					{
					case 0: ReadConfig(DefualtCfg); break;
					case 1: ReadConfig(LegitCfg); break;
					case 2: ReadConfig(RageCfg); break;
					}
				}

				ImGui::SameLine();

				if (Button(xorstr_("Save Config"), ImVec2(178, 0)))
				{
					switch (iCurrentConfig)
					{
					case 0: SetConfig(DefualtCfg); break;
					case 1: SetConfig(LegitCfg); break;
					case 2: SetConfig(RageCfg); break;
					}
				}

				Bar();

				ImGui::SetCursorPosY(280);
				ImGui::PushItemWidth(175);
				if (Button(xorstr_("Reset Features"), ImVec2(364, 0)))
				{
					ResetSettings();
					bReset = true;
				}

				ImGui::PopItemWidth();
				ImGui::PushItemWidth(175);
				if (Button(xorstr_("Refresh"), ImVec2(364, 0)))
					bUpdate = true;
				ImGui::PopItemWidth();
				if (Button(xorstr_("End Cheat"), ImVec2(178, 0)))
					bEnd = true;
				ImGui::SameLine();
				if (Button(xorstr_("End Menu"), ImVec2(178, 0)))
					bEndUM = true;
				PopFont();
			}
			ImGui::PopFont();
		}
		EndChild();
	}
	End();

	return;
}