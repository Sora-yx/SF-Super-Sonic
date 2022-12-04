#include "pch.h"

//Big shoutout to Death for helping finding the proper function that set SS!!!

FUNCTION_PTR(char, __fastcall, TriggerSuperSonic, sigTriggerSS(), size_t* a1, bool enabled);

size_t* sonicContextPtr = nullptr;

//we hack the function that manage MSG for Sonic as we need the SonicContext instance to call Super Sonic
HOOK(char, __fastcall, PlayerStateProcessMSG_r, sigPStateProcessMSG(), size_t* SonicContext, __int64 a2, __int64 a3)
{
	size_t curAction = *(DWORD*)(a2 + 8);
	PrintInfo("cur Action: %d", curAction);
	sonicContextPtr = SonicContext;

	if (GetKeyState('S') & 0x8000)
	{
		TriggerSuperSonic(SonicContext, true);
	}

	if (GetKeyState('Z') & 0x8000)
	{
		TriggerSuperSonic(SonicContext, false);
	}

	return originalPlayerStateProcessMSG_r(SonicContext, a2, a3);
}

void init_SuperSonicHacks()
{
	INSTALL_HOOK(PlayerStateProcessMSG_r);
}