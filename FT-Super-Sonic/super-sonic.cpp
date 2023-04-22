#include "pch.h"
#include "input.h"
#include "xinput.h"
#include "music.h"

//Big shoutout to Death for helping finding the proper function that trigger SS!!!

SonicContext* sonicContextPtr = nullptr;
SSEffAuraS* auraPtr = nullptr;
static int ringTimer = 0;
bool isSuper = false;
char statusBackup[0x180] = { 0 };
uint8_t inputDelay = 0;
int currentIsland = 0;

//we hack the function that check if the player is Super Sonic to copy SonicContext instance to call Super Sonic later
//
HOOK(bool, __fastcall, isSuperSonic_r, sigIsSuperSonic(), SonicContext* sCont)
{
	sonicContextPtr = sCont;
	isSuper = originalisSuperSonic_r(sCont);
	return isSuper;
}

HOOK(SSEffAuraS*, __fastcall, SuperSonicEffectAura_r, sigsub_SSEFfectAura(), SSEffAuraS* ptrSSAura)
{
	auraPtr = originalSuperSonicEffectAura_r(ptrSSAura);
	return auraPtr;
}

void Transfo(SonicContext* SContext)
{
	memcpy(statusBackup, SContext->pBlackBoardStatus, sizeof(BlackboardStatus));
	app::player::TriggerSuperSonic(SContext, true);
}

void Untransfo(SonicContext* SContext)
{
	app::player::TriggerSuperSonic(SContext, false);
	memcpy(SContext->pBlackBoardStatus, statusBackup, sizeof(BlackboardStatus)); //fix floaty physics when detransform
}


void Transfo_CheckInput(SonicContext* SContext)
{
	if (inputDelay)
	{
		inputDelay--;
		return;
	}

	if ((isKeyPressed(UntransformKey) || isInputPressed(UntransformBtn)) && isSuper) //detransfo
	{
		if (!app::player::SetSonicFall(SContext, 0))
			return;

		if (auraPtr)
		{
			app::player::SSAuraDestructor(auraPtr);
		}

		RestoreOriginalMusic();
		Untransfo(SContext);

		return;
	}
	else
	{
		auto ring = app::player::GetRings(SContext);

		if ((isKeyPressed(TransformKey) || isInputPressed(TransformBtn)) && !isSuper && (nolimit || ring >= 50))
		{
			PlayMusic();
			Transfo(SContext);
			return;
		}
	}
}

void ringLoss(SonicContext* SContext)
{
	if (nolimit || !isSuper)
		return;

	auto ring = app::player::GetRings(SContext);

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

void Ascend_CheckInput(StructAB* a)
{
	if (isKeyPressed(AscendKey) || isInputPressed(AscendBtn))
	{
		a->spdY = 35.0f;
	}
}

void Descend_CheckInput(StructAB* a)
{
	if (isKeyPressed(DescendKey) || isInputPressed(DescendBtn))
	{
		a->spdY = -35.0f;
	}
}

void SuperSonic_OnFrames(SonicContext* SContext)
{
	//!isInGame() ||
	if (!SContext || !SContext->pSonic || (size_t*)!SContext->pGOCPlayerKinematicPrams)
		return;

	size_t* param = (size_t*)SContext->pGOCPlayerKinematicPrams;
	Transfo_CheckInput(SContext);

	if (isSuper)
	{
		ringLoss(SContext);
		Ascend_CheckInput((StructAB*)param);
		Descend_CheckInput((StructAB*)param);
	}
}

void RemoveRings(SonicContext* SContext)
{
	SubRing(SContext, 1);
}

HOOK(__int64, __fastcall, ChangeStateParameter_r, sigChangeStateParameter(), __int64 a1, __int64 a2, __int64 a3)
{
	PrintInfo("Set New Stage Param: %d\n", a2);
	return originalChangeStateParameter_r(a1, a2, a3);
}

int oldMsg = -1;
HOOK(char, __fastcall, PlayerStateProcessMSG_r, sigPStateProcessMSG(), SonicContext* SContext, Message* a2, __int64 a3)
{
	auto id = a2->msgID;

	if (id != oldMsg)
	{
		//PrintInfo("new msg: %d\n", id);
		oldMsg = id;
	}

	return originalPlayerStateProcessMSG_r(SContext, a2, a3);
}

//sig scan doesn't seem to work for that one 
HOOK(__int64, __fastcall, GetCurIsland_r, 0x140222FC0, __int64 a1)
{
	currentIsland = originalGetCurIsland_r(a1);
	return currentIsland;
}

void init_SuperSonicHacks()
{
	INSTALL_HOOK(isSuperSonic_r);
	WRITE_NOP(sigsub_IsNotInCyber(), 0x2); //force Super Sonic visual to be loaded in cyberspace (fix crash)
	INSTALL_HOOK(SuperSonicEffectAura_r); //used to delete super aura when detransform
	INSTALL_HOOK(GetCurIsland_r);

	//used for research atm, todo: delete after
	INSTALL_HOOK(ChangeStateParameter_r);
	INSTALL_HOOK(PlayerStateProcessMSG_r);
}