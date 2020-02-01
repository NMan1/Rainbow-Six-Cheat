#include <Windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <shlobj.h>
#include "Settings.h"

using namespace g_Settings;
void Keys();

class Config 
{

public:
	Config(std::string name) 
	{
		TCHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS, NULL, 0, path)))
		{
			std::string PathToFile = std::string(path) + "\\" + name;
			this->path = PathToFile; //Set path to cfg file
			this->input = std::ifstream(this->path); //open input
		}
	}

	void open() 
	{
		this->outputstream = std::ofstream(this->path); //open output for config writing
	}

	void close() 
	{
		this->outputstream.close(); //close output after writing config
	}

	template <typename tp>
	tp set(std::string name, tp value) {
		outputstream << name << " " << value << std::endl; //Write to config
		return true;
	}

	template <class tg>
	tg get(std::string name) {
		this->input.close(); //Close Input
		this->input = std::ifstream(this->path); //Jump back to idx 0 of txt file
		std::vector<std::string>stack;
		std::string buffer;
		while (this->input >> buffer) {
			stack.push_back(buffer); //push all options into vector
		}
		for (int i = 0; i < stack.size(); i++) {
			if (stack.at(i) == name) {

				return atof(stack.at(i + 1).c_str()); //if option found get value from idx + 1
			}
		}
	}
private:
	std::string path;
	std::ifstream input;
	std::ofstream outputstream;
};

void static SetConfig(Config* config)
{
	config->open();
	config->set<bool>("bOutline", bOutline);
	config->set<bool>("bTeam", bTeam);
	config->set<bool>("bChams", bChams);
	config->set<bool>("bCavEsp", bCavEsp);	

	config->set<bool>("bOutline", bOutline);
	config->set<bool>("bTeam", bTeam);
	config->set<bool>("bChams", bChams);
	config->set<bool>("bCavEsp", bCavEsp);	

	config->set<int>("iAimSelection", iAimSelection);
	config->set<int>("iAimBone", iAimBone);
	config->set<int>("iAimKey2", iAimKey2);
	config->set<int>("iAimKey", iAimKey);
	config->set<int>("iSilentKey", iSilentKey);

	config->set<bool>("bAimbot", bAimbot);
	config->set<int>("iAimFov", iAimFov);
	config->set<bool>("bSmoothing", bSmoothing);
	config->set<bool>("bSilent", bSilent);
	config->set<float>("iSmoothing", iSmoothing);
	config->set<bool>("bRunShoot", bRunShoot);
	config->set<bool>("bDamage", bDamage);
	config->set<bool>("bNoRecoil", bNoRecoil);
	config->set<bool>("bSpread", bSpread);

	config->set<bool>("bUnlockAll", bUnlockAll);
	config->set<bool>("bColor", bColor);
	config->set<int>("iNoClip", iNoClip);
	config->set<bool>("bNoClip", bNoClip);
	config->set<bool>("bWepFov", bWepFov);
	config->set<bool>("bPlayerFov", bPlayerFov);
	config->set<bool>("bSpeed", bSpeed);
	config->set<bool>("bNoFlash", bNoFlash);

	config->set<float>("fSpread", fSpread);
	config->set<float>("fWepFov", fWepFov);
	config->set<float>("fPlayerFov", fPlayerFov);
	config->set<float>("fValue", fValue);
	config->set<int>("iSpeed", iSpeed);
	config->set<int>("iBullet", iBullet);
	config->set<float>("fBullet", fBullet);

	config->set<float>("fChamsR", fChamsR);
	config->set<float>("fChamsG", fChamsG);
	config->set<float>("fChamsB", fChamsB);
	config->set<float>("fChamsOpp", fChamsOpp);
	config->set<float>("fOpp", fOpp);

	config->close();
}

void static ReadConfig(Config* config)
{

	bOutline = config->get<bool>("bOutline");
	bTeam = config->get<bool>("bTeam");
	bChams = config->get<bool>("bChams");
	bCavEsp = config->get<bool>("bCavEsp");

	bOutline = config->get<bool>("bOutline");
	bTeam = config->get<bool>("bTeam");
	bChams = config->get<bool>("bChams");
	bCavEsp = config->get<bool>("bCavEsp");

	iAimSelection = config->get<int>("iAimSelection");
	iAimBone = config->get<int>("iAimBone");
	iAimKey2 = config->get<int>("iAimKey2");
	iAimKey = config->get<int>("iAimKey");
	iSilentKey = config->get<int>("iSilentKey");

	bAimbot = config->get<bool>("bAimbot");
	iAimFov = config->get<int>("iAimFov");
	bSmoothing = config->get<bool>("bSmoothing");
	bSilent = config->get<bool>("bSilent");
	iSmoothing = config->get<float>("iSmoothing");
	bRunShoot = config->get<bool>("bRunShoot");
	bDamage = config->get<bool>("bDamage");
	bNoRecoil = config->get<bool>("bNoRecoil");
	bSpread = config->get<bool>("bSpread");

	bUnlockAll = config->get<bool>("bUnlockAll");
	bColor = config->get<bool>("bColor");
	iNoClip = config->get<int>("iNoClip");
	bNoClip = config->get<bool>("bNoClip");
	bWepFov = config->get<bool>("bWepFov");
	bPlayerFov = config->get<bool>("bPlayerFov");
	bSpeed = config->get<bool>("bSpeed");
	bNoFlash = config->get<bool>("bNoFlash");

	fSpread = config->get<float>("fSpread");
	fWepFov = config->get<float>("fWepFov");
	fPlayerFov = config->get<float>("fPlayerFov");
	fValue = config->get<int>("fValue");
	iSpeed = config->get<int>("iSpeed");
	iBullet = config->get<int>("iBullet");
	fBullet = config->get<float>("fBullet");

	fChamsR = config->get<float>("fChamsR");
	fChamsG = config->get<float>("fChamsG");
	fChamsB = config->get<float>("fChamsB");
	fChamsOpp = config->get<float>("fChamsOpp");
	fOpp = config->get<float>("fOpp");

	g_Settings::bUpdateSettings = true;
}

extern Config* DefualtCfg;
extern Config* LegitCfg;
extern Config* RageCfg;