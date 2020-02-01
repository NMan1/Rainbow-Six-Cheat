#include "Authentication.h"
#include <iostream>
#include "GUI.h"
#include "Settings.h"
#include <vector>
#include <time.h>
#include <ctime>
#include <sstream>
#include <shlobj.h>
#include <fstream>
#include <ostream>
#include "xor.h"
#include <d3d9.h>
#include <stdlib.h>
#pragma comment(lib, "mysqlcppconn8-static.lib")

bool Auth::Authenitcation = false;
bool Auth::bHWID = false;
bool bSavePass;
bool bShowPass = false;
bool bRenew = false; 
auto flags =  ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;
auto flagsPass = ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

int uid = 0;
char* pUser = {};
char* pPass = {};

char pUser2[12]{ (char)pUser };
char pPass2[12]{ (char)pPass };

std::string sUser;
std::string sPass;

struct Date
{
	int d, m, y;
};

const int monthDays[12] = { 31, 28, 31, 30, 31, 30,
						   31, 31, 30, 31, 30, 31 };


void Menu::RenderLogin()
{
	// Style
	auto& style = GetStyle();

	// Menu
	SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(320, 230));
	static const auto dwFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;
	Begin(xorstr_("###Overflow"), 0, dwFlags);
	{
		//ImGui::Image((void*)Menu::LogoTexture, ImVec2(180, 180));

		// Multi Colored Rect
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImColor c = ImColor(32, 114, 247);
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 22), ImVec2(p.x + ImGui::GetWindowWidth() - 16, p.y), ImColor(30, 30, 39));
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 22), ImVec2(p.x + ImGui::GetWindowWidth() - 16, p.y + 24), ImColor(234, 57, 57, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));
		Spacing(); //22 , 24

		// Tilte
		PushFont(Font18);
		Text(xorstr_("Overflow "));
		SameLine();
		PushStyleColor(ImGuiCol_Text, TabTitleText);
		Text(Status.c_str());
		PopStyleColor();
		Spacing(); 
		Spacing(); 

		if (!g_Settings::bDisplayLogin)
		{
			SetCursorPosY(GetCursorPosY() - 10);
			BeginChild(xorstr_("Login"), ImVec2(0, 190), true);
			{
				static bool doonce = true;
				if (bSavePass && doonce)
				{
					doonce = false;
					for (int i = 0; i < sUser.length(); i++)
						pUser2[i] = sUser[i];

					for (int i = 0; i < sPass.length(); i++)
						pPass2[i] = sPass.at(i);
				}

				Text(xorstr_("Username"));
				if (g_Settings::bPassFailed)
				{
					SameLine();
					Text(" -");
					SameLine();
					PushStyleColor(ImGuiCol_Text, RedText);
					Text(xorstr_(" Wrong username/password!"));
					PopStyleColor();
				}

				if (Auth::bHWID)
				{
					static bool DoOnce = true;
					if (DoOnce)
						ImGui::OpenPopup(xorstr_("Account HWID Locked"));

					if (ImGui::BeginPopupModal(xorstr_("Account HWID Locked"), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
					{
						ImGui::Separator();
						Spacing();
						Spacing();

						PushFont(Font16);
						ImGui::Text(xorstr_("Your account has been hwid locked."));
						ImGui::Text(xorstr_("\ndue to another login on a diffrent PC.\nmessage NMan1#4443 on discord to unlock it.\n"));
						PopFont();

						Spacing();
						Spacing();
						ImGui::Separator();

						ImGui::SetCursorPosX(104);
						Spacing();
						if (ImGui::Button(xorstr_("Close"), ImVec2(120, 0)))
						{
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
						DoOnce = false;
					}
				}

				if (ImGui::InputText(xorstr_(""), pUser2, sizeof(pUser2), flags))
					g_Settings::sUsername = pUser2;

				Text("Password");
				if (ImGui::InputText(xorstr_("###  "), pPass2, sizeof(pPass2), flagsPass))
					g_Settings::sPass = pPass2;

				if (GetAsyncKeyState(VK_RETURN))
				{
					g_Settings::sUsername = pUser2;
					g_Settings::sPass = pPass2;
				}

				if (bShowPass)
					flagsPass = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;
				else
					flagsPass = ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

				Spacing();

				Checkbox(xorstr_("Show Password"), &bShowPass);
				SameLine();
				Checkbox(xorstr_("Save Password"), &bSavePass);

				Spacing();

				if (Button(xorstr_("Login"), ImVec2(140.5, 25)))
				{
					g_Settings::sUsername = pUser2;
					g_Settings::sPass = pPass2;
				}

				SameLine();

				if (Button(xorstr_("Close"), ImVec2(140.5, 25)))
					g_Settings::bEndUM = true;
			}
			EndChild();
		}	
		else if (g_Settings::bDisplayLogin)
		{
			// now with correct user&pass write to file
			if (bSavePass)
				Auth::SavePassword(false);

			BeginChild(xorstr_("#### "), ImVec2(0, 49.5), true);
			{
				Text("UID: %d", uid);
				if (g_Settings::iTimeLeft <= 0)
				{
					PushStyleColor(ImGuiCol_Text, RedText);
					Text(xorstr_("License: 0 Days"));
					PopStyleColor();
				}
				else
				{
					PushStyleColor(ImGuiCol_Text, GreenText);
					Text(xorstr_("License: %d Days"), g_Settings::iTimeLeft);
					PopStyleColor();
				}
			}
			EndChild();		
			Dummy(ImVec2(0, 1));
			SetCursorPosY(98);
			BeginChild(xorstr_("Success"), ImVec2(0, 124.3), true);
			{
				PushStyleColor(ImGuiCol_Text, GreenText);
				Text(xorstr_("Login Success!"));
				PopStyleColor();

				PushFont(Font16);
				Text(xorstr_("Launch the game after clicking inject..."));
				PopFont();

				Spacing();
				Spacing();				
				Spacing();
				Spacing();
				Spacing();

				Text(xorstr_("License:"));

				if (g_Settings::iTimeLeft <= 0)
				{
					static bool DoOnce = true;
					if (DoOnce)
						ImGui::OpenPopup(xorstr_("License Expired"));

					if (ImGui::BeginPopupModal(xorstr_("License Expired"), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
					{
						ImGui::Separator();
						Spacing();
						Spacing();

						PushFont(Font16);
						ImGui::Text(xorstr_("Your Licnese has expired."));
						ImGui::Text(xorstr_("\nTo renew your license,\nmessage NMan1#4443 on discord.\n"));
						PopFont();

						Spacing();
						Spacing();
						ImGui::Separator();

						ImGui::SetCursorPosX(104);
						Spacing();
						if (ImGui::Button(xorstr_("Close"), ImVec2(120, 0)))
						{
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
						DoOnce = false;
					}

					PushStyleColor(ImGuiCol_Text, RedText);
					Text(xorstr_("0 Days Remaining"));
					PopStyleColor();
				}
				else
				{
					PushStyleColor(ImGuiCol_Text, GreenText);
					Text(xorstr_("%d Days Remaining"), g_Settings::iTimeLeft);
					PopStyleColor();
				}

				ImVec2 p = ImGui::GetCursorScreenPos();
				ImGui::SetCursorPosY(p.y - 14); // 16
				if (Button(xorstr_("Renew"), ImVec2(140.5, 25)))
				{
					ImGui::OpenPopup(xorstr_("Renew"));
				}
				
				SetNextWindowPos(ImVec2(8, 42));
				ImGui::SetNextWindowSize(ImVec2(304, 180));
				if (ImGui::BeginPopupModal(xorstr_("Renew"), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
				{
					ImGui::Separator();
					Spacing();
					Spacing();

					PushFont(Font16);
					ImGui::Text(xorstr_("To renew your license or extend its length,\nmessage NMan1#4443 (owner) on discord."));
					PopFont();

					Spacing();
					Spacing();
					ImGui::Separator();

					if (ImGui::Button(xorstr_("Close"), ImVec2(120, 0)))
					{
						ImGui::CloseCurrentPopup(); 
					}
					ImGui::EndPopup();
				}

				SameLine();
				ImGui::SetCursorPosX(156);
				if (g_Settings::iTimeLeft > 0 && g_Settings::iTimeLeft < 3000)
				{
					if (Button(xorstr_("Inject"), ImVec2(140.5, 25)))
						Auth::Authenitcation = true;
				}
				else 
				{
					if (Button(xorstr_("Close"), ImVec2(140.5, 25)))
						g_Settings::bEndUM = true;
				}
			}
			EndChild();
		}

		PopFont();
	}
	ImGui::End();
	return;
}

int countLeapYears(Date d)
{
	int years = d.y;
	if (d.m <= 2)
		years--;
	return years / 4 - years / 100 + years / 400;
}

int getDifference(Date dt1, Date dt2)
{
	long int n1 = dt1.y * 365 + dt1.d;
	for (int i = 0; i < dt1.m - 1; i++)
		n1 += monthDays[i];
	n1 += countLeapYears(dt1);
	long int n2 = dt2.y * 365 + dt2.d;
	for (int i = 0; i < dt2.m - 1; i++)
		n2 += monthDays[i];
	n2 += countLeapYears(dt2);

	return (n2 - n1);
}

//mysql = Year - Month - Date
bool Auth::CheckLicense(std::string sUserDate)
{
	// Get Current Time
	time_t Time = time(0);
	struct tm* Today = localtime(&Time);
	int Year = Today->tm_year + 1900;
	int Month = Today->tm_mon + 1;
	int Day = Today->tm_mday;

	std::string stringUserYear, stringUserMonth;
	int userYear, userMonth, userDay;

	std::stringstream iUserYear(stringUserYear.append(sUserDate.begin(), std::find(sUserDate.begin(), sUserDate.end(), '-')));
	sUserDate.erase(0, iUserYear.str().length() + 1);
	std::stringstream iUserMonth(stringUserMonth.append(sUserDate.begin(), std::find(sUserDate.begin(), sUserDate.end(), '-')));
	std::stringstream iUserDay(sUserDate.erase(0, iUserMonth.str().length() + 1));

	iUserYear >> userYear;
	iUserMonth >> userMonth;
	iUserDay >> userDay;

	Date CurrentDate = { Day, Month, Year };
	Date UserDate	 = { userDay, userMonth, userYear };

	g_Settings::iTimeLeft = getDifference(CurrentDate, UserDate);
	if (g_Settings::iTimeLeft < 0 || g_Settings::iTimeLeft > 3000)
		return false;
	else if (g_Settings::iTimeLeft >= 0)
		return true;

	return false;
}

void Auth::SavePassword(bool Load)
{
	TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS, NULL, 0, path)))
	{
		std::string PathToFile = std::string(path) + xorstr_("\\save.txt");
		std::fstream fFile(PathToFile);
		if (!fFile.is_open()) // if cant find file create one
		{
			std::ofstream oFile(PathToFile.c_str());
			oFile.close();
			fFile.close();
		}
		else // there is a file
		{
			if (Load)
			{
				int i = 0;  std::string sLine;
				while (std::getline(fFile, sLine))
				{
					if (sLine.length() > 0)
						bSavePass = true;
					if (!i)
						sUser = sLine;				
					else if (i)
					{
						sPass = sLine;
						fFile.close();
						break;
					}
					i++;
				}
			}
			else
			{
				std::ofstream oFile(PathToFile.c_str());
				std::string data(g_Settings::sUsername); oFile << data.c_str() << std::endl;
				std::string data3(g_Settings::sPass);    oFile << data3.c_str() << std::endl;
				oFile.close();
			}
			fFile.close();
		}
	}
}

bool Auth::HWID(int id, sql::SQLString sqlHWID, sql::Driver* driver, sql::Connection* con, sql::PreparedStatement* pstmt)
{
	// Tester account return true
	if (sqlHWID == "2")
		return true;	

	// Account uses spoofer
	if (sqlHWID == "3")
		return true;

	// HWID already set
	if (sqlHWID.length() > 1)
	{
		HW_PROFILE_INFO hwProfileInfo;
		if (GetCurrentHwProfile(&hwProfileInfo))
		{
			if (hwProfileInfo.szHwProfileGuid == sqlHWID)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	else // need to set hwid
	{
		HW_PROFILE_INFO hwProfileInfo;
		if (GetCurrentHwProfile(&hwProfileInfo))
		{
			//update
			pstmt = con->prepareStatement(xorstr_("UPDATE customers SET customerhwid = ? WHERE customersid = ?"));
			pstmt->setString(1, hwProfileInfo.szHwProfileGuid);
			pstmt->setInt(2, id);
			pstmt->executeQuery();

			return true;
		}
	}

	return false;
}
void Auth::Login()
{
	const std::string server = xorstr_("");
	const std::string username = xorstr_("");
	const std::string password = xorstr_("");

	sql::Driver* driver;
	sql::Connection* con;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;

	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
	//	std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
		exit(1);
	}

	con->setSchema(xorstr_("users"));

	//select  
	pstmt = con->prepareStatement(xorstr_("SELECT * FROM customers;"));
	result = pstmt->executeQuery();

	static bool bleave = false;
	do
	{
		while (result->next())
		{
			if (g_Settings::sUsername == result->getString(3)) // Username
			{
				if (g_Settings::sPass == result->getString(4))  // password
				{
					if (Auth::HWID(result->getInt(1), result->getString(2), driver, con, pstmt))
					{
						if (!CheckLicense(result->getString(5)))
							g_Settings::iTimeLeft = 0;

						uid = result->getInt(1);

						g_Settings::bDisplayLogin = true;
						bleave = true;
						break;
					}
					else
						Auth::bHWID = true; break;
				}
				else if (g_Settings::sPass.size() > 0)
					g_Settings::bPassFailed = true;
			}
			else if (g_Settings::sUsername.size() > 0)
				g_Settings::bPassFailed = true;
		}

		pstmt = con->prepareStatement(xorstr_("SELECT * FROM customers;"));
		result = pstmt->executeQuery();
	} while (!bleave);
	
	delete result;
	delete pstmt;
	delete con;
	return;
}

