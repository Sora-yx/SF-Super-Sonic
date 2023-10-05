#include "pch.h"
#include "ss.h"
#include <vector>
#include "music.h"
#include <iostream>
#include <windows.h>
#include "bass_vgmstream.h"

HANDLE stdoutHandle = nullptr;
static bool inGame = false;
int currentIsland = 0;
__int64* MsgPtr = nullptr;

enum msg
{
	MsgBegingTalkNpc = 8271,
	MsgTransitCyberStage = 9080,
	MsgTransitHacking = 9083,
	MsgTransitIsland,
	MsgTransitMasterTrialStage,
	MsgTransitMenu,
	MsgTransitPractice,
	MsgEndCyber = 9061,
	Msgpause = 9178, //updated but not sure, rest need to be done
	MsgEndPhaseBRush = 8427, //more like end regular combat I think
};

void PrintInfo(const char* text, ...)
{
	va_list ap;
	va_start(ap, text);

	if (!stdoutHandle)
		stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	char buffer[512];
	_vsprintf_p(buffer, 512, text, ap);
	printf("%s%s\n", "[Super Sonic]: ", buffer);
	va_end(ap);
}

bool isInGame()
{
	return inGame;
}

static bool triggerEnableGame = false;
static int timerDelay = 0;
void SetInGameFalse()
{
	inGame = false;
	timerDelay = 0;
	triggerEnableGame = false;
}


void SetInGameTrue()
{
	if (triggerEnableGame)
	{
		if (++timerDelay == 130)
		{
			inGame = true;
			timerDelay = 0;
			triggerEnableGame = false;
		}
	}	
}

#pragma region GameState Hooks

HOOK(GameModeStagePlay*, __fastcall, GameStatePlayAllocator_r, 0x1401F05D0, GameModeStagePlay* a1)
{
	ResumeBassMusic();
	triggerEnableGame = true;
	return  originalGameStatePlayAllocator_r(a1);;
}

HOOK(__int64, __fastcall, CyberSpacePlayStateAllocator_r, 0x14771EBA0, __int64 a1)
{
	ResumeBassMusic();
	triggerEnableGame = true;
	return  originalCyberSpacePlayStateAllocator_r(a1);
}

HOOK(__int64, __fastcall, CyberStageChallengePlayState_Allocator_r, 0x1401BB730, __int64 a1)
{
	ResumeBassMusic();
	triggerEnableGame = true;
	return originalCyberStageChallengePlayState_Allocator_r(a1);
}

HOOK(__int64, __fastcall, GetCurIsland_r, 0x14025F8C0, __int64 a1)
{
	currentIsland = originalGetCurIsland_r(a1);
	return currentIsland;
}

#pragma endregion


std::vector<int64_t> msgV;

static bool isValid(int64_t msg)
{
	for (int i = 0; i < msgV.size(); i++)
	{
		if (msgV.at(i) == msg)
		{
			return true;
		}
	}

	msgV.push_back(msg);
	return false;
}

HOOK(__int64, __fastcall, SetNewMSG_r, sigSetNewMsg(), __int64* a1, __int64 msgID)
{
	if (!isValid(msgID))
	{
		//PrintInfo("New msg ID: %d\n", msgID);
	}

	if (msgID == MsgBegingTalkNpc)
	{
		if (isSuper)
			ForceUnTransfo(false);
	}
	else if (msgID == Msgpause) 
	{
		PauseBassMusic();
		ResetValues();
	}
	else if ( (msgID >= MsgTransitCyberStage && msgID <= MsgTransitPractice))
	{
		StopBassMusic();
		ResetValues();
	}

	if (msgID == 8640 || msgID == 9072) //end / fail cyberspace
	{
		if (isSuper)
		{
			ForceUnTransfo(true);
		}
		else
		{
			ResetValues();
		}
	}
	
	return originalSetNewMSG_r(a1, msgID);
}

int GetKey(std::string s)
{
	std::string copy = s;

	std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);

	if (copy.find("space") != std::string::npos)
	{
 		return VK_SPACE;
	}
	else if (copy.find("ctrl") != std::string::npos)
	{
		return VK_CONTROL;
	}

	return s[0];
}

std::string findFile(const std::string& folderPath, const std::string& fileName) 
{

	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA((folderPath + "/*").c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return "";
	}

	do 
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			continue;
		}

		std::string currentFileName = findData.cFileName;
		
		if (currentFileName.find(fileName) != std::string::npos) 
		{
			std::string filePath = folderPath + "/" + currentFileName;
			FindClose(hFind);
			return filePath;
		}
	} while (FindNextFileA(hFind, &findData) != 0);

	FindClose(hFind);
	return "";
}


void Init_Util()
{
	INSTALL_HOOK(GetCurIsland_r);

	//Used to check if the game is on a state "playable" and unpaused, hopefully someday I'll find a more convenient way to do it, LOL
	INSTALL_HOOK(GameStatePlayAllocator_r);
	INSTALL_HOOK(CyberSpacePlayStateAllocator_r);

	INSTALL_HOOK(CyberStageChallengePlayState_Allocator_r);

	//used to check that the game has been paused
	INSTALL_HOOK(SetNewMSG_r);

	//battle rush has many issue with SS so not allowing it there for now.

	//INSTALL_HOOK(BattleRushPlayStateAllocator_r);	
	//INSTALL_HOOK(BattleRushNextPhase_r); //force SS to detransfo between battle rush transition (fix crashes)

}
