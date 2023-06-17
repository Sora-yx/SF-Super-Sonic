#include "pch.h"

HANDLE stdoutHandle = nullptr;
static bool inGame = false;
int currentIsland = 0;

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

HOOK(GameModeStagePlay*, __fastcall, GameStatePlayDestructor_r, sigEndGameStatePlay(), __int64 a1, __int64 a2)
{
	inGame = false;
	return originalGameStatePlayDestructor_r(a1, a2);
}

//failed to get sig for that one (40 53 \n 48 83 EC 20)
HOOK(__int64, __fastcall, CyberSpacePlayStateAllocator_r, 0x147416ED0, __int64 a1)
{
	inGame = true;
	return originalCyberSpacePlayStateAllocator_r(a1);
}

//failed to get sig for that one (48 83 EC 28 \n 48 8B 0D AD 46 9D 03)
HOOK(__int64, __fastcall, CyberSpacePlayStateDestructor_r, 0x140187EF0)
{
	inGame = false;
	return originalCyberSpacePlayStateDestructor_r();
}

HOOK(__int64, __fastcall, BattleRushPlayStateAllocator_r, 0x14017D560, __int64 a1)
{
	inGame = true;
	return originalBattleRushPlayStateAllocator_r(a1);
}

HOOK(__int64, __fastcall, BattleRushPlayStateDestructor_r, 0x14017ECF0, __int64 a1, __int64 a2)
{
	inGame = false;
	return originalBattleRushPlayStateDestructor_r(a1, a2);
}


HOOK(__int64, __fastcall, CyberStageChallengePlayState_Allocator_r, 0x14018C170, __int64 a1)
{
	inGame = true;
	return originalCyberStageChallengePlayState_Allocator_r(a1);
}


HOOK(char, __fastcall, CyberStageChallengePlayStateDestructor_r, 0x140187EF0)
{
	inGame = false;
	return originalCyberStageChallengePlayStateDestructor_r();
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


void Init_Util()
{
	INSTALL_HOOK(GetCurIsland_r);
	//Used to check if the game is on a state "playable", hopefully someday I'll find a more convenient way to do it, LOL
	INSTALL_HOOK(GameStatePlayAllocator_r);
	INSTALL_HOOK(CyberSpacePlayStateAllocator_r);
	INSTALL_HOOK(BattleRushPlayStateAllocator_r);	
	INSTALL_HOOK(CyberStageChallengePlayState_Allocator_r);

	INSTALL_HOOK(GameStatePlayDestructor_r);
	INSTALL_HOOK(CyberSpacePlayStateDestructor_r);
	INSTALL_HOOK(BattleRushPlayStateDestructor_r);
	INSTALL_HOOK(CyberStageChallengePlayStateDestructor_r);
}