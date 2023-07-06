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
	Msgpause = 9114,
	MsgEndPhaseBRush = 8427, //more like end regular combat I think
	MsgEndCyber = 8578,

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
		if (++timerDelay == 50)
		{
			inGame = true;
			timerDelay = 0;
			triggerEnableGame = false;
		}
	}	
}

#pragma region GameState Hooks


//failed to get sigscan :(
HOOK(GameModeStagePlay*, __fastcall, GameStatePlayAllocator_r, 0x1401CD4E0, GameModeStagePlay* a1)
{
	ResumeBassMusic();
	triggerEnableGame = true;
	return  originalGameStatePlayAllocator_r(a1);;
}

//failed to get sig for that one (40 53 \n 48 83 EC 20)
HOOK(__int64, __fastcall, CyberSpacePlayStateAllocator_r, 0x1473AB790, __int64 a1)
{
	ResumeBassMusic();
	triggerEnableGame = true;
	return  originalCyberSpacePlayStateAllocator_r(a1);
}

HOOK(__int64, __fastcall, CyberStageChallengePlayState_Allocator_r, 0x14019B860, __int64 a1)
{
	ResumeBassMusic();
	triggerEnableGame = true;
	return originalCyberStageChallengePlayState_Allocator_r(a1);
}

//sig scan doesn't seem to work for that one  ( 8B 81 A8 00 00 00 )
HOOK(__int64, __fastcall, GetCurIsland_r, 0x14023A250, __int64 a1)
{
	currentIsland = originalGetCurIsland_r(a1);
	return currentIsland;
}

HOOK(__int64, __fastcall, TrainingStart_r, 0x1401AFB30, __int64 a1, __int64 a2)
{
	resetSonicContextPtr();
	return originalTrainingStart_r(a1, a2);
}


HOOK(__int64, __fastcall, TrainingStart2_r, 0x14764D920, __int64 a1)
{
	resetSonicContextPtr();
	return originalTrainingStart2_r(a1);
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

	return false;
}


HOOK(__int64, __fastcall, SetNewMSG_r, sigSetNewMsg(), __int64* a1, __int64 msgID)
{
	if (msgID == Msgpause) 
	{
		PauseBassMusic();
		resetSonicContextPtr();
	}

	if (msgID == MsgEndCyber) 
	{
		if (isSuper)
		{
			ForceUnTransfo(true);
		}
		else
		{
			resetSonicContextPtr();
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

void SetInfiniteBoost()
{
	WRITE_NOP(sig_SetBoostValue(), 0x5);
}

void DisableInfiniteBoost()
{
	intptr_t address = (intptr_t)sig_SetBoostValue();
	WRITE_MEMORY(address, uint8_t, 0xF3);
	WRITE_MEMORY(address + 1, uint8_t, 0xF);
	WRITE_MEMORY(address + 2, uint8_t, 0x11);
	WRITE_MEMORY(address + 3, uint8_t, 0x4F);
	WRITE_MEMORY(address + 4, uint8_t, 0x3C);
}

void signalHandler(int signal) 
{
	PrintInfo("Program is closing.");

	BASS_Free();
	exit(signal);
}

void Init_Util()
{
	INSTALL_HOOK(GetCurIsland_r);

	//Used to check if the game is on a state "playable" and unpaused, hopefully someday I'll find a more convenient way to do it, LOL
	INSTALL_HOOK(GameStatePlayAllocator_r);
	INSTALL_HOOK(CyberSpacePlayStateAllocator_r);

	INSTALL_HOOK(CyberStageChallengePlayState_Allocator_r);

	INSTALL_HOOK(TrainingStart_r);
	INSTALL_HOOK(TrainingStart2_r);

	//used to check that the game has been paused
	INSTALL_HOOK(SetNewMSG_r);

	//battle rush has many issue with SS so not allowing it there for now.

	//INSTALL_HOOK(BattleRushPlayStateAllocator_r);	
	//INSTALL_HOOK(BattleRushNextPhase_r); //force SS to detransfo between battle rush transition (fix crashes)
}
