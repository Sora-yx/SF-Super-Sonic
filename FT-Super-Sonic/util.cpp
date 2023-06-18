#include "pch.h"
#include "ss.h"
#include <vector>

HANDLE stdoutHandle = nullptr;
static bool inGame = false;
int currentIsland = 0;


enum msg
{
	Msgpause = 9064,
	MsgEndPhaseBRush = 8427,
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

#pragma region GameState Hooks


HOOK(GameModeStagePlay*, __fastcall, GameStatePlayAllocator_r, 0x1401B5D40, GameModeStagePlay* a1)
{
	inGame = true;
	return originalGameStatePlayAllocator_r(a1);
}


//failed to get sig for that one (40 53 \n 48 83 EC 20)
HOOK(__int64, __fastcall, CyberSpacePlayStateAllocator_r, 0x147416ED0, __int64 a1)
{
	inGame = true;
	return originalCyberSpacePlayStateAllocator_r(a1);
}


HOOK(__int64, __fastcall, BattleRushPlayStateAllocator_r, 0x14017D560, __int64 a1)
{
	inGame = true;
	return originalBattleRushPlayStateAllocator_r(a1);
}

HOOK(__int64, __fastcall, CyberStageChallengePlayState_Allocator_r, 0x14018C170, __int64 a1)
{
	inGame = true;
	return originalCyberStageChallengePlayState_Allocator_r(a1);
}

//sig scan doesn't seem to work for that one  ( 8B 81 A8 00 00 00 )
HOOK(__int64, __fastcall, GetCurIsland_r, 0x140222FC0, __int64 a1)
{
	currentIsland = originalGetCurIsland_r(a1);
	return currentIsland;
}

#pragma endregion

uint8_t CheckStatusFieldFlags(int64_t in_field, uint32_t in_flags)
{
	return _bittest64(&in_field, in_flags);
}

std::vector<int> PrevMsg;

bool isValid(int msg)
{
	for (uint16_t i = 0; i < PrevMsg.size(); i++)
	{
		if (PrevMsg[i] == msg)
		{
			return false;
		}
	}

	PrevMsg.push_back(msg);
	return true;
}

HOOK(__int64, __fastcall, SetNewMSG_r, sigSetNewMsg(), __int64 a1, __int64 msgID)
{
	if (msgID == Msgpause) 
	{
		inGame = false;
	}

	if (msgID == MsgEndCyber) 
	{
		if (isSuper)
		{
			ForceUnTransfo();
		}

		inGame = false;
	}

	if (isValid(msgID))
	{
		PrintInfo("new msg: %d", msgID);
	}
	
	return originalSetNewMSG_r(a1, msgID);
}

HOOK(__int64, __fastcall, BattleRushNextPhase_r, sigBattleRushNextPhase(), __int64 a1, __int64 a2)
{
	if (isSuper)
	{
		ForceUnTransfo();
	}

	inGame = false;
	return originalBattleRushNextPhase_r(a1, a2);
}

void Init_Util()
{
	INSTALL_HOOK(GetCurIsland_r);

	//Used to check if the game is on a state "playable" and unpaused, hopefully someday I'll find a more convenient way to do it, LOL
	INSTALL_HOOK(GameStatePlayAllocator_r);
	INSTALL_HOOK(CyberSpacePlayStateAllocator_r);
	INSTALL_HOOK(BattleRushPlayStateAllocator_r);	
	INSTALL_HOOK(CyberStageChallengePlayState_Allocator_r);

	//used to check that the game has been paused
	INSTALL_HOOK(SetNewMSG_r);

	INSTALL_HOOK(BattleRushNextPhase_r); //force SS to detransfo between battle rush transition (fix crashes)


}