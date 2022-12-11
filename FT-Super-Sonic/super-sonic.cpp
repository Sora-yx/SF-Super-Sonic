#include "pch.h"

//Big shoutout to Death for helping finding the proper function that trigger SS!!!

FUNCTION_PTR(char, __fastcall, TriggerSuperSonic, sigTriggerSS(), SonicContext* a1, bool enabled);
FUNCTION_PTR(size_t, __fastcall, GetRings, sigGetRings(), SonicContext* sContext);
FUNCTION_PTR(void, __fastcall, SubRing, sigSubRings(), SonicContext* sContext, int count);
FUNCTION_PTR(void, __fastcall, SetNextAnim, 0x1407A7710, __int64 a1, const char* action, unsigned __int8 a3);
FUNCTION_PTR(void, __fastcall, SetAura, 0x14078EFD0, __int64 a1, bool a2);
FUNCTION_PTR(char**, __cdecl, Gocplayereffect, 0x14078F590, void);
FUNCTION_PTR(Sonic*, __fastcall, GetPSonic, 0x1401F22D0, SonicContext* a1);
FUNCTION_PTR(__int64, __fastcall, sub_140B8B120, 0x140B8B120, Sonic* a1, __int64 a2);
FUNCTION_PTR(void*, __cdecl, sub_1407A8490, 0x1407A8490, void);
FUNCTION_PTR(char, __fastcall, SetSonicFall, 0x14086EC50, SonicContext* a1, char a2);
FUNCTION_PTR(__int64, __fastcall, sub_14078F7E0, 0x14078F7E0, __int64 a1, __int64 id);
FUNCTION_PTR(char, __fastcall, ChangeStateParameter, 0x140798A50, SonicContext* Sonk, __int64 actionID, __int64 a3);
FUNCTION_PTR(char, __fastcall, sub_140865810, 0x140865810, SonicContext* a1);

SonicContext* sonicContextPtr = nullptr;
SSEffAuraS* auraPtr = nullptr;
static int ringTimer = 0;
bool isSuper = false;
bool transfoAllowed = false;
char statusBackup[0x180] = { 0 };

HOOK(SSEffAuraS*, __fastcall, SuperSonicEffectAura, 0x140783CE0, SSEffAuraS* ptrSSAura, __int64 a2, Sonic* Sonk, __int64 a4)
{
	auraPtr = originalSuperSonicEffectAura(ptrSSAura, a2, Sonk, a4);
	return auraPtr;
}

void Untransfo(SonicContext* SContext)
{
	TriggerSuperSonic(SContext, false);

	if (!SetSonicFall(SContext, 0))
	{
		ChangeStateParameter(SContext, 1, 1);
	}
}

void Transfo_CheckInput(SonicContext* SContext)
{
	if (GetKeyState('Z') & 0x8000 && isSuper) //detransfo
	{
		Untransfo(SContext);
		memcpy(SContext->pBlackBoardStatus, statusBackup, sizeof(BlackboardStatus)); //fix floaty physics when detransform
		return;
	}

	auto ring = GetRings(SContext);

	if (GetKeyState('Y') & 0x8000 && !isSuper && (nolimit || ring >= 50))
	{
		//changeSSMusic();
		memcpy(statusBackup, SContext->pBlackBoardStatus, sizeof(BlackboardStatus));
		TriggerSuperSonic(SContext, true);

		if (auraPtr)
			auraPtr->AuraFlagMaybe &= 1u;

		return;
	}
}

void ringLoss(SonicContext* SContext)
{
	if (nolimit || !isSuper)
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

void SuperSonic_OnFrames(SonicContext* SContext)
{
	if (!SContext || !SContext->pSonic)
		return;

	Transfo_CheckInput(SContext);
	ringLoss(SContext);
}

void RemoveRings(SonicContext* SContext)
{
	SubRing(SContext, 1);
}

void changeSSMusic();

//we hack the function that manage MSG for Sonic as we need the SonicContext instance to call Super Sonic
HOOK(char, __fastcall, PlayerStateProcessMSG_r, sigPStateProcessMSG(), SonicContext* SContext, __int64 a2, __int64 a3)
{
	sonicContextPtr = SContext;
	return originalPlayerStateProcessMSG_r(SContext, a2, a3);
}


HOOK(__int64, __fastcall, ChangeStateParameter_r, 0x140798A50, __int64 a1, __int64 a2, __int64 a3)
{
	PrintInfo("Set New Stage Param: %d\n", a2);
	return originalChangeStateParameter_r(a1, a2, a3);
}

HOOK(StateFly*, __fastcall, PlayerStateFlyCOnstructor, 0x140821690, __int64 a1)
{
	return originalPlayerStateFlyCOnstructor(a1);
}

HOOK(char, __fastcall, SetSuperSonicNextAction_r, 0x14086EE40, SonicContext* a1, char a2, unsigned int a3)
{
	return originalSetSuperSonicNextAction_r(a1, a2, a3);
}

HOOK(bool, __fastcall, isSuperSonic_r, 0x140798FF0, SonicContext* pSonic)
{
	isSuper = originalisSuperSonic_r(pSonic);
	return isSuper;
}

void init_SuperSonicHacks()
{
	INSTALL_HOOK(PlayerStateProcessMSG_r);
	INSTALL_HOOK(ChangeStateParameter_r);
	INSTALL_HOOK(SetSuperSonicNextAction_r);
	INSTALL_HOOK(isSuperSonic_r);
	INSTALL_HOOK(SuperSonicEffectAura);
}