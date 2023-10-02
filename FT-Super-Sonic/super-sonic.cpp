#include "pch.h"
#include "input.h"
#include "xinput.h"
#include "music.h"
#include "BlackboardHelper.h"
#include "ss.h"

//Big shoutout to Death for helping finding the proper function that trigger SS!!!

SonicContext* sonicContextPtr = nullptr;
SSEffAuraS* auraPtr = nullptr;
static int ringTimer = 0;
bool isSuper = false;
char statusBackup = 0;
uint8_t inputDelay = 0;
int curState = 0;
//we hack the function that checks if the player is Super Sonic to copy SonicContext instance

HOOK(bool, __fastcall, isSuperSonic_r, sigIsSuperSonic(), SonicContext* sCont)
{
	sonicContextPtr = sCont;
	return originalisSuperSonic_r(sCont);
}

//copy aura instance to disable it when detransform as the game doesn't do it.
HOOK(SSEffAuraS*, __fastcall, SuperSonicEffectAura_r, sigsub_SSEFfectAura(), SSEffAuraS* ptrSSAura)
{
	auraPtr = originalSuperSonicEffectAura_r(ptrSSAura);
	return auraPtr;
}

void SuperSonic::Transfo(SonicContext* SContext)
{
	statusBackup = SContext->pBlackBoardStatus->pad35[26];
	TriggerSuperSonic(SContext, true);
}

void SuperSonic::Untransfo(SonicContext* SContext)
{
	TriggerSuperSonic(SContext, false);
	SContext->pBlackBoardStatus->pad35[26] = statusBackup;  //fix floaty physics when detransform
}

void ForceUnTransfo(bool resetValues)
{
	if (!sonicContextPtr || !isInGame())
		return;

	SetSonicFall(sonicContextPtr, 0);

	if (auraPtr)
	{
		SSAuraDestructor(auraPtr);
	}

	SuperSonic::Untransfo(sonicContextPtr);


	if (resetValues)
	{
		ResetValues();
	}
	else
	{
		ChangeStateParameter(sonicContextPtr, 1, 0);
	}

	RestoreOriginalMusic();
}

static bool PlayerpressedTransfoBtn = false;

//TO Do: Use classes directly from the game to avoid that lol
void ResetValues()
{
	SetInGameFalse();
	PlayerpressedTransfoBtn = false;
	isSuper = false;
}

void SuperSonic::Transfo_CheckInput(SonicContext* SContext)
{
	if (!SContext)
		return;

	if (inputDelay)
	{
		inputDelay--;
		return;
	}

	if ((isKeyPressed(UntransformKey) || isInputPressed(UntransformBtn)) && isSuper && BlackboardHelper::IsFlyingAsSS()) //detransfo
	{
		if (!SetSonicFall(SContext, 0))
			return;

		if (auraPtr)
		{
			SSAuraDestructor(auraPtr);
		}

		ForceUnTransfo(false);

		return;
	}
	else
	{


		auto ring = GetRings(SContext);

		if ((isKeyPressed(TransformKey) || isInputPressed(TransformBtn)) && !isSuper && (nolimit || ring >= 50))
		{
			
			if (( (BlackboardHelper::IsJumping() || BlackboardHelper::IsFalling()) && !PlayerpressedTransfoBtn))
			{
				ChangeStateParameter(SContext, 1, 0u); //force Sonk to stand state to remove jump ball effect
				PlayerpressedTransfoBtn = true; //delay a bit the transfo
				return;
			}
		}

		if (PlayerpressedTransfoBtn)
		{
			SuperSonic::Transfo(SContext);
			PlayerpressedTransfoBtn = false;
			PlayMusic();
			return;
		}
	}
}

void SuperSonic::ringLoss(SonicContext* SContext)
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
			ForceUnTransfo(false);
		}

		ringTimer = 0;
		return;
	}
}

void SuperSonic::Ascend_CheckInput(SonicContext* sonk, GOCKinematicPrams* a)
{
	if (isKeyPressed(AscendKey) || isInputPressed(AscendBtn))
	{	
		a->spdY = 50.0f;
	}
}

void SuperSonic::Descend_CheckInput(GOCKinematicPrams* a)
{
	if (isKeyPressed(DescendKey) || isInputPressed(DescendBtn))
	{
		a->spdY = -50.0f;
	}
}

void SuperSonic::OnFrames()
{
	if (BlackboardHelper::GetStatus() == nullptr)
		return;

	if (BlackboardHelper::IsDead())
	{
		ResetValues();
		return;
	}

	SetInGameTrue();

	auto SContext = sonicContextPtr;

	if (!SContext || !SContext->pSonic)
		return;

	auto stat = BlackboardHelper::GetStatus();



	isSuper = BlackboardHelper::IsSuper();

	GOCKinematicPrams* param = (GOCKinematicPrams*)SContext->pGOCPlayerKinematicPrams;

	SuperSonic::Transfo_CheckInput(SContext);

	if (isSuper)
	{
		SuperSonic::ringLoss(SContext);

		if (BlackboardHelper::IsFlyingAsSS())
		{
			SuperSonic::Ascend_CheckInput(SContext, param);
			SuperSonic::Descend_CheckInput(param);
		}
	}
}

void RemoveRings(SonicContext* SContext)
{
	SubRing(SContext, 1);
}

HOOK(char, __fastcall, ChangeStateParameter_r, 0x1408AAB30, SonicContext* a1, __int64 a2, __int64 a3)
{
	curState = a2;
	return originalChangeStateParameter_r(a1, a2, a3);
}

//unused, originally made to fix T-pose SS when removing Flying state, but I ended not doing that.
const char* getSSAnim(const char* anm)
{
	std::string string = anm;

	if (string == "RUNNING" || string == "RUNNING_BATTLE")
	{
		anm = "FLY";
	}

	return anm;
}

//unused
HOOK(void, __fastcall, SetNextAnim_r, 0x1407C9280, __int64 a1, const char* a2, unsigned __int8 a3)
{	
	if (isSuper)
	{
		a2 = getSSAnim(a2);
	}

	originalSetNextAnim_r(a1, a2, a3);
}

//used during Titan fight to set SS and ring drain
HOOK(char, __fastcall, titanfightCheck_r, sig_TitanSSManage(), __int64 a1, __int64 a2, float a3)
{
	//we don't want the custom SS stuff like ring drain to work during Titan fights.
	SetInGameFalse();
	return originaltitanfightCheck_r(a1, a2, a3);
}

void SuperSonic::Init() 
{

	WRITE_NOP(sigIsNotCyberSpace(), 0x2); //force Super Sonic visual to be loaded in cyberspace (fix crash)

	INSTALL_HOOK(isSuperSonic_r);
	INSTALL_HOOK(SuperSonicEffectAura_r); //used to delete super aura when detransform
	INSTALL_HOOK(ChangeStateParameter_r);

	INSTALL_HOOK(titanfightCheck_r);
}