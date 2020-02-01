#include <iostream>
#include <thread>
#include <mutex>
#include "Authentication.h"
#include "Cheat.h"
#include "GUI.h"
#include "Settings.h"
#include "Rights.h"
#include <ShlObj_core.h>
#include "xor.h"
#include "Config.h"

// Namespaces
using namespace Menu;
using namespace g_Settings;
using namespace g_Cheat;

// Helpers
std::string Pass;
bool bSetupMenu = true;
void LocateDriver();

int main()
{
	// hide files
	SHELLSTATE ss;
	ZeroMemory(&ss, sizeof(ss));
	ss.fShowAllObjects = FALSE;
	ss.fShowSysFiles = FALSE;
	SHGetSetSettings(&ss, SSF_SHOWALLOBJECTS | SSF_SHOWSYSFILES | SSF_SHOWSUPERHIDDEN, TRUE);

	// Hide console
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	// Setup Menu
	Menu::SetUpLoginMenu();

	// Find saved password
	Auth::SavePassword(true);

	// Start Login
	std::thread DoLoginCheck(Auth::Login);

	// After succsess from login and driver handle start main cheat thread
	std::thread DoCheat(Cheat::MainCheat);

	// Main Login loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		// Setup Input
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		if (!Auth::Authenitcation)
		{
			Menu::BeginDraw();
			Menu::RenderLogin();
			Menu::EndDraw();
		}
		else if (Auth::Authenitcation)
			break;

		if (bEndUM)
		{
			Menu::MenuShutDown();
			return 0;
		}
	}

	std::thread KeysLoop(Keys);

	if (Auth::Authenitcation)
	{
		Menu::MenuShutDown();

		if (!Driver.OpenFile())
		{
			system(xorstr_("Installer.exe"));
			LocateDriver();
		}

		while (true)
		{
			if (Driver.IsGameOpen())
				break;

			Sleep(500);
		}

		g_Settings::bStartCheat = true;

		if (Auth::Authenitcation)
		{
			// Unallocate old menu recourses
			Menu::SetupMenu();

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (true)
			{
				// Setup Input
				if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
					continue;
				}

				// Render main cheat menu if in the game
				Menu::BeginDraw();
				Menu::RenderMenu();
				Menu::EndDraw();

				// End UM & Driver
				if (bCheatThread)
				{
					Driver.End();
					break;
				}

				if (bEndUM)
					break;
			}
		}
	}

	DefualtCfg->close();
	LegitCfg->close();
	RageCfg->close();

	Driver.CloseHandles(); // End Proper Driver Handles
	Menu::MenuShutDown();  // End Menu Allocations and Handles

	return 0;
}

void LocateDriver()
{
	bool bStatus = false;
	do
	{
		bStatus = Driver.OpenFile();
	} while (!bStatus);
}