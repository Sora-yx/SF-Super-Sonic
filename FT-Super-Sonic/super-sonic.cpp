#include "pch.h"

//Big shoutout to Death for helping finding the proper function that set SS!!!

FUNCTION_PTR(char, __fastcall, TriggerSuperSonic, sigTriggerSS(), SonicContext* a1, bool enabled);

SonicContext* sonicContextPtr = nullptr;
StatePluginBossBattle* pluginBoss = nullptr;
bool isSuper = false;

FUNCTION_PTR(__int64, __fastcall, sub_14086C710, 0x14086C710, SonicContext* a1, __int64 a2);
FUNCTION_PTR(__int64, __fastcall, sub_140817880, 0x140817880, SonicContext* a1);
FUNCTION_PTR(void, __fastcall, sub_14079C720, 0x14079C720, __int64 a1, __int64 a2);
FUNCTION_PTR(char, __fastcall, sub_1407EFB20, 0x1407EFB20, SonicContext* SonicCOntext, __int64 a2, __int64 a3);
FUNCTION_PTR(size_t, __fastcall, GetRings, 0x140798CE0, SonicContext* sContext);
FUNCTION_PTR(void, __fastcall, SubRing, 0x140799740, SonicContext* sContext, int count);

HOOK(StatePluginBossBattle*, __fastcall, StatePluginBossBattleConst_r, 0x140851810, StatePluginBossBattle* a1, __int64 a2)
{
	StatePluginBossBattle* toto = originalStatePluginBossBattleConst_r(a1, a2);
	pluginBoss = toto;
	return toto;
}

void Untransfo(SonicContext* SContext)
{
	TriggerSuperSonic(SContext, false);
	isSuper = false;
}

static int ringTimer = 0;
void SuperSonic_OnFrames(SonicContext* SContext)
{
	if (nolimit || !isSuper || !SContext)
		return;

	auto ring = GetRings(SContext);

	if (++ringTimer == 60)
	{
		if (ring > 0)
		{
			SubRing(SContext, 1);
		}
		else
		{
			Untransfo(SContext);
		}

		ringTimer = 0;
		return;
	}
}

void changeSSMusic();
void Transfo_CheckInput(SonicContext* SContext)
{
	auto ring = GetRings(SContext);

	if (GetKeyState('S') & 0x8000 && !isSuper && (nolimit || ring >= 50)) 
	{
		//changeSSMusic();
		TriggerSuperSonic(SContext, true);
		isSuper = true;
		return;
	}

	if (GetKeyState('Z') & 0x8000 && isSuper) //detransfo
	{
		Untransfo(SContext);
		return;
	}
}

//we hack the function that manage MSG for Sonic as we need the SonicContext instance to call Super Sonic
HOOK(char, __fastcall, PlayerStateProcessMSG_r, sigPStateProcessMSG(), SonicContext* SContext, __int64 a2, __int64 a3)
{
	size_t curAction = *(DWORD*)(a2 + 8);
	PrintInfo("cur Action: %d", curAction);
	sonicContextPtr = SContext;

	Transfo_CheckInput(SContext);

	return originalPlayerStateProcessMSG_r(SContext, a2, a3);
}

void init_SuperSonicHacks()
{
	INSTALL_HOOK(PlayerStateProcessMSG_r);
	INSTALL_HOOK(StatePluginBossBattleConst_r);
}