#include "pch.h"

HANDLE stdoutHandle = nullptr;
bool IsGameStatePlay = false;
bool isCyberStatePlay = false;

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
	return IsGameStatePlay || isCyberStatePlay;
}

HOOK(StatePlay*, __fastcall, GameModePlayStateConstructor_r, 0x1401A1F00, StatePlay* a1)
{
	IsGameStatePlay = true;
	return originalGameModePlayStateConstructor_r(a1);
}

HOOK(__int64, __fastcall, EndPlayStateMaybe_r, sigEndPlayStateMaybe(), __int64 a1, __int64 a2)
{
	IsGameStatePlay = false;
	return originalEndPlayStateMaybe_r(a1, a2);
}

HOOK(CyberStatePlay*, __fastcall, CyberStatePlayConstructor_r, 0x14748CC20, CyberStatePlay* a1)
{
	isCyberStatePlay = true;
	return originalCyberStatePlayConstructor_r(a1);
}

HOOK(__int64, __cdecl, EndCyberStatePlayMaybe_r, 0x14017AFF0, void)
{
	isCyberStatePlay = false;
	return originalEndCyberStatePlayMaybe_r();
}

void init_Util()
{
	//Used to check if the game is on a state "playable", hopefully someday I'll find a more convenient way to do it, LOL
	INSTALL_HOOK(GameModePlayStateConstructor_r);
	INSTALL_HOOK(EndPlayStateMaybe_r);
	INSTALL_HOOK(CyberStatePlayConstructor_r);
	INSTALL_HOOK(EndCyberStatePlayMaybe_r);
}