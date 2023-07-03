#include "pch.h"
#include "ss.h"
#include <vector>
#include "music.h"

HANDLE stdoutHandle = nullptr;
static bool inGame = false;
int currentIsland = 0;
__int64* MsgPtr = nullptr;

enum msg
{
	Msgpause = 9064,
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

void SetInGameFalse()
{
	inGame = false;
}

#pragma region GameState Hooks


//failed to get sigscan :(
HOOK(GameModeStagePlay*, __fastcall, GameStatePlayAllocator_r, 0x1401CD4E0, GameModeStagePlay* a1)
{
	inGame = true;
	ResumeBassMusic();
	return originalGameStatePlayAllocator_r(a1);
}

//failed to get sig for that one (40 53 \n 48 83 EC 20)
HOOK(__int64, __fastcall, CyberSpacePlayStateAllocator_r, 0x1473AB790, __int64 a1)
{
	inGame = true;
	ResumeBassMusic();
	return originalCyberSpacePlayStateAllocator_r(a1);
}


HOOK(__int64, __fastcall, CyberStageChallengePlayState_Allocator_r, 0x14019B860, __int64 a1)
{
	inGame = true;
	ResumeBassMusic();
	return originalCyberStageChallengePlayState_Allocator_r(a1);
}

//sig scan doesn't seem to work for that one  ( 8B 81 A8 00 00 00 )
HOOK(__int64, __fastcall, GetCurIsland_r, 0x14023A250, __int64 a1)
{
	currentIsland = originalGetCurIsland_r(a1);
	return currentIsland;
}

#pragma endregion


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
			ForceUnTransfo();
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
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);

	if (s.find("space") != std::string::npos)
	{
 		return VK_SPACE;
	}
	else if (s.find("ctrl") != std::string::npos)
	{
		return VK_CONTROL;
	}

	char ch = s.c_str()[0];
	return ch;
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
