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
bool isSS2 = false;
char statusBackup[10] = { 0 };
uint8_t inputDelay = 0;
int curState = 0;
//we hack the function that checks if the player is Super Sonic to copy SonicContext instance

//support for "Hedgehog May Cry" mod 
static const int timerUnTransfoHedgeMayCryCD = 60 * 45;
static int timerUnTransfoHedgeMayCry = 0;

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


void SuperSonic::TransfoSS2(SonicContext* SContext)
{	
	auto pPlayer = PlayerHsmContextGetPlayer((long long*)SContext);
	auto pPlayerVisualGocStaticClass = GOCPlayerVisualGetStaticClass();
	
	if (pPlayer && pPlayerVisualGocStaticClass)
	{
		auto pPlayerVisualGoc = GameObjectGetGoc(pPlayer, (__int64)pPlayerVisualGocStaticClass);

		auto pVisualSuperSonic = GOCPlayerVisualGetVisualByHashName(
			pPlayerVisualGoc,
			*(int*)pVisualSuperSonicHash);

		if (pVisualSuperSonic)
			VisualChangeToSuperSonic2(pVisualSuperSonic);

		isSS2 = true;
	}
}

void SuperSonic::UntransfoSS2(SonicContext* SContext)
{
	if (!isSS2)
		return;

	auto pPlayer = PlayerHsmContextGetPlayer((long long*)SContext);
	auto pPlayerVisualGocSaticClass = GOCPlayerVisualGetStaticClass();

	if (pPlayer && pPlayerVisualGocSaticClass)
	{
		auto pPlayerVisualGoc = GameObjectGetGoc(pPlayer, (__int64)pPlayerVisualGocSaticClass);

		auto pVisualSuperSonic = GOCPlayerVisualGetVisualByHashName(
			pPlayerVisualGoc,
			*(int*)pVisualSuperSonicHash);

		if (pVisualSuperSonic)
			VisualChangeToSuperSonic(pVisualSuperSonic);

		isSS2 = false;
	}

}

void SuperSonic::Transfo(SonicContext* SContext)
{
	//statusBackup = SContext->pBlackBoardStatus->pad35[26];
	timerUnTransfoHedgeMayCry = 0;
	memcpy(statusBackup, SContext->pBlackBoardStatus->pad35, sizeof(statusBackup));
	TriggerSuperSonic(SContext, true);
}

void SuperSonic::Untransfo(SonicContext* SContext)
{
	timerUnTransfoHedgeMayCry = 0;
	TriggerSuperSonic(SContext, false);
	memcpy(SContext->pBlackBoardStatus->pad35, statusBackup, sizeof(statusBackup));
	//SContext->pBlackBoardStatus->pad35[26] = statusBackup;  //fix floaty physics when detransform
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

	SuperSonic::UntransfoSS2(sonicContextPtr);
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
		if (isSS2)
		{
			UntransfoSS2(SContext);
			return;
		}

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

		if ((isKeyPressed(TransformKey) || isInputPressed(TransformBtn)))
		{

			if (!isSuper)
			{
				if ((hedgeMayCry && ring >= 100 || !hedgeMayCry && nolimit || !hedgeMayCry && ring >= 50))
				{
					if (((BlackboardHelper::IsJumping() || BlackboardHelper::IsFalling()) && !PlayerpressedTransfoBtn))
					{
						ChangeStateParameter(SContext, 1, 0u); //force Sonk to stand state to remove jump ball effect
						PlayerpressedTransfoBtn = true; //delay a bit the transfo
						return;
					}
				}
			}
			else
			{
				if (!isSS2)
				{
					if ((hedgeMayCry && ring >= 200 || !hedgeMayCry && nolimit || !hedgeMayCry && ring >= 100))
					{
						SuperSonic::TransfoSS2(SContext);
						return;
					}
				}
			}

		}

		if (PlayerpressedTransfoBtn)
		{
			if (!isSuper)
			{
				SuperSonic::Transfo(SContext);
				PlayerpressedTransfoBtn = false;
				PlayMusic();
				return;
			}
		}
	}
}

void SuperSonic::ringLoss(SonicContext* SContext)
{
	if (nolimit || !isSuper || hedgeMayCry)
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

static const float spdCap = 130.0f;
static const float minimSpd = 20.0f;
static float ascendSpd = minimSpd;

void SuperSonic::Ascend_CheckInput(SonicContext* sonk, GOCKinematicPrams* a)
{
	if (isKeyPressed(AscendKey) || isInputPressed(AscendBtn))
	{	
		a->spdY = ascendSpd < spdCap ? ascendSpd++ : ascendSpd;
	}
	else
	{
		if (ascendSpd > minimSpd)
			ascendSpd--;
		else
			ascendSpd = minimSpd;
	}
}

static const float minimSpdDesc = -20.0f;
static const float spdCapDesc = -130.0f;
static float descendSpd = minimSpdDesc;

void SuperSonic::Descend_CheckInput(GOCKinematicPrams* a)
{
	if (isKeyPressed(DescendKey) || isInputPressed(DescendBtn))
	{
		a->spdY = descendSpd > spdCapDesc ? descendSpd-- : descendSpd;
	}
	else
	{
		if (descendSpd < minimSpdDesc)
			descendSpd++;
		else
			descendSpd = minimSpdDesc;
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

	if (!isInGame() || !SContext || !SContext->pSonic)
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

			if (hedgeMayCry)
			{
				if (timerUnTransfoHedgeMayCry < timerUnTransfoHedgeMayCryCD)
				{
					timerUnTransfoHedgeMayCry++;
				}
				else
				{
					ForceUnTransfo(false);
					timerUnTransfoHedgeMayCry = 0;
					return;
				}
			}
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

void SuperSonic::InitSS2()
{
	//0x140151BF0
	//SS2 research
	WRITE_NOP(0x14B754155, 0x2); //force pac file to always load
}

void SuperSonic::Init() 
{
	//WRITE_NOP(0x1409C3067, 0x6);
	WRITE_NOP(sigIsNotCyberSpace(), 0x2); //force Super Sonic visual to be loaded in cyberspace (fix crash)

	INSTALL_HOOK(isSuperSonic_r);
	INSTALL_HOOK(SuperSonicEffectAura_r); //used to delete super aura when detransform
	INSTALL_HOOK(ChangeStateParameter_r);

	INSTALL_HOOK(titanfightCheck_r);

	//SuperSonic:InitSS2();
}