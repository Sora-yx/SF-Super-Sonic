#include "pch.h"

//Big shoutout to Death for helping finding the proper function that trigger SS!!!

FUNCTION_PTR(char, __fastcall, TriggerSuperSonic, sigTriggerSS(), SonicContext* a1, bool enabled);
FUNCTION_PTR(size_t, __fastcall, GetRings, sigGetRings(), SonicContext* sContext);
FUNCTION_PTR(void, __fastcall, SubRing, sigSubRings(), SonicContext* sContext, int count);
FUNCTION_PTR(char, __fastcall, SetSonicFall, sigSetSonicFall(), SonicContext* a1, char a2);
FUNCTION_PTR(char, __fastcall, ChangeStateParameter, sigChangeStateParameter(), SonicContext* Sonk, __int64 actionID, __int64 a3);

SonicContext* sonicContextPtr = nullptr;
SSEffAuraS* auraPtr = nullptr;
static int ringTimer = 0;
bool isSuper = false;
bool transfoAllowed = false;
char statusBackup[0x180] = { 0 };

//we hack the function that check if the player is Super Sonic to copy SonicContext instance to call Super Sonic later
HOOK(bool, __fastcall, isSuperSonic_r, sigIsSuperSonic(), SonicContext* sCont)
{
	sonicContextPtr = sCont;
	isSuper = originalisSuperSonic_r(sCont);
	return isSuper;
}

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
	if (!isInGame() || !SContext || !SContext->pSonic)
		return;

	Transfo_CheckInput(SContext);
	ringLoss(SContext);
}

void RemoveRings(SonicContext* SContext)
{
	SubRing(SContext, 1);
}

void changeSSMusic();

HOOK(__int64, __fastcall, ChangeStateParameter_r, sigChangeStateParameter(), __int64 a1, __int64 a2, __int64 a3)
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

HOOK(void, __fastcall, SetNextAnim_r, 0x1407A7710, __int64 a1, const char* anim, unsigned __int8 a3)
{
	return originalSetNextAnim_r(a1, anim, a3);
}


void init_SuperSonicHacks()
{
	INSTALL_HOOK(isSuperSonic_r);
	WRITE_NOP(sigIsNotCyberSpace(), 0x2); //force Super Sonic visual to be loaded in cyberspace (fix crash)

	//used for research atm, todo: delete after
	INSTALL_HOOK(ChangeStateParameter_r);
	INSTALL_HOOK(SetSuperSonicNextAction_r);
	INSTALL_HOOK(SuperSonicEffectAura);
	INSTALL_HOOK(SetNextAnim_r);
}