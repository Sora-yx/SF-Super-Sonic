#include "pch.h"
#include "input.h"
#include "xinput.h"
#include "music.h"
#include "BlackboardHelper.h"
#include "ss.h"

//Big shoutout to Death for helping finding the proper function that trigger SS!!!

SSEffAuraS* auraPtr = nullptr;
static int ringTimer = 0;
bool isSuper = false;
bool isSS2 = false;
int64_t statusBackup = 0;
uint8_t inputDelay = 0;
int curState = 0;
bool titanFight = false;

//support for "Hedgehog May Cry" mod 
static const int timerTransfoBackHMC_CD = 60 * 90;
static const int timerUnTransfoHMC_CD = 60 * 45;
static int timerUnTransfoHMC = 0;
static int timerTransfoBackHMC = 0;

static int statusBackupTest[sizeof(Blackboardstatus)] = {0};
static int64_t worldflagbackup = 0;

SonicContext* SuperSonic::GetSonicContext()
{
	auto p = (Sonic*)BlackboardHelper::GetPlayer();

	if (p)
	{
		auto context = p->sonkContext;

		if (context)
			return context;
	}

	return nullptr;
}


//copy aura instance to disable it when detransform as the game doesn't do it.
HOOK(SSEffAuraS*, __fastcall, SuperSonicEffectAura_r, sigsub_SSEFfectAura(), SSEffAuraS* ptrSSAura)
{
	auraPtr = originalSuperSonicEffectAura_r(ptrSSAura);
	return auraPtr;
}

bool isPerfectParry = false;
void DisablePerfectParry(SonicContext* SContext)
{
	if (isPerfectParry)
	{
		DisableCombatFlags((long long*)SContext, 16i64);
		DisableWorldFlags((long long*)SContext, 0x47u);
	}
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


		if (isPerfectParry)
		{
			SetCombatFlags((long long*)SContext, 16i64, 1);
			SetWorldFlags((long long*)SContext, 71i64, 1);
		}

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

		DisablePerfectParry(SContext);
		isSS2 = false;
	}
}

void SuperSonic::Transfo(SonicContext* SContext)
{
	timerUnTransfoHMC = 0;
	TriggerSuperSonic(SContext, true);
}

void SuperSonic::Untransfo(SonicContext* SContext)
{
	TriggerSuperSonic(SContext, false);
	SContext->pBlackBoardStatus->WorldFlags = statusBackup; //fix floaty physics when detransform
}

void ForceUnTransfo(bool resetValues)
{
	auto context = SuperSonic::GetSonicContext();

	if (!context)
		return;

	SuperSonic::UntransfoSS2(context);

	SetSonicFall(context, 0);

	if (auraPtr)
	{
		SSAuraDestructor(auraPtr);
	}

	SuperSonic::Untransfo(context);

	if (resetValues)
	{
		ResetValues();
	}
	else
	{
		ChangeStateParameter(context, 1, 0);
	}

	RestoreOriginalMusic();
}

static bool PlayerpressedTransfoBtn = false;
extern bool photoMode;
//TO Do: Use classes directly from the game to avoid that lol
void ResetValues()
{
	SetInGameFalse();
	titanFight = false;
	PlayerpressedTransfoBtn = false;
	isSuper = false;
	isSS2 = false;
	photoMode = false;
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
		if (hedgeMayCry && !titanFight)
		{
			if (timerTransfoBackHMC > 0)
			{
				timerTransfoBackHMC--;
				return;
			}
		}

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
	if (nolimit || !isSuper || hedgeMayCry || titanFight)
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

static const float spdCap = 160.0f;
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
static const float spdCapDesc = -160.0f;
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

	if (BlackboardHelper::IsDead()) //might be not needed anymore
	{
		ResetValues();
		return;
	}

	SetInGameTrue();

	auto SContext = SuperSonic::GetSonicContext();

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
				if (timerUnTransfoHMC < timerUnTransfoHMC_CD)
				{
					timerUnTransfoHMC++;
				}
				else
				{
					ForceUnTransfo(false);
					timerTransfoBackHMC = timerTransfoBackHMC_CD;
					timerUnTransfoHMC = 0;
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

//to do test if it works fine
HOOK(char, __fastcall, ChangeStateParameter_r, sig_ChangeStateParameter(), SonicContext* a1, __int64 a2, __int64 a3)
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

//very hack but it works, not feeling nopping all the condition checks and restoring them after.
void SetSS2ParryAnim(const char* &a2)
{
	if (isSuper && isSS2)
	{
		if (strcmp(a2, "PARRY_MISS") == NULL) //check if the const char contains at least "parry_miss"
		{
			a2 = "PARRY_JUST_MISS";
		}
		else if (strcmp(a2, "PARRY_START") == NULL)
		{
			a2 = "PARRY_JUST_START";
		}
		else if (strstr(a2, "PARRY") != NULL || strstr(a2, "PARRY_FLY") != NULL)  //check if the const char matches a specific value 
		{
			a2 = "PARRY_JUST";
		}
	}
}

//sig scan seems to  fail for the set anims functions.

//"\x40\x53\x48\x83\xEC\x20\xF6\x81\x78\x01\x00\x00\x10\x48\x8B", "xxxxxxxxxxxxxxx"
HOOK(void, __fastcall, SetNextAnim_r, 0x1408BB260, __int64 a1, const char* a2, unsigned __int8 a3)
{	
	SetSS2ParryAnim(a2);
	originalSetNextAnim_r(a1, a2, a3);
}

//"\xE8\xCC\xCC\xCC\xCC\xEB\x7E\x8B\x4E\x50", "x????xxxxx"
HOOK(void, __fastcall, SetNextAnim2_r, 0x1408BB2F0, __int64 a1, const char* a2)
{
	SetSS2ParryAnim(a2);
	originalSetNextAnim2_r(a1, a2);
}

//I made some tests and couldn't find where this is used, but the parry code reference it, so just in case...
//"\x40\x53\x48\x83\xEC\x20\x0F\xBE\xD2\x49\x8B\xD8\xE8\xCC\xCC\xCC\xCC\x48\x8B\x48\x30\x48\x85\xC9\x74\x0D\x48\x8B\xD3\x48\x83\xC4\x20\x5B\xE9\xCC\xCC\xCC\xCC\x48", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxx????x"
HOOK(void, __fastcall, SetNextAnim3_r, 0x1409D8220, __int64 a1, char a2, const char* a3)
{	
	SetSS2ParryAnim(a3);
	originalSetNextAnim3_r(a1, a2, a3);
}

//used during Titan fight to set SS and ring drain
HOOK(void, __fastcall, titanfightCheck_r, sig_TitanSSManage(), __int64 a1, __int64 a2, float a3)
{
	//we don't want the custom SS stuff like ring drain to work during Titan fights.
	titanFight = true;
	return originaltitanfightCheck_r(a1, a2, a3);
}

//updated
HOOK(size_t, __fastcall, CreateSonicContext_r, 0x14AB3D5E0, size_t* Sonk, __int64 a2)
{
	isSuper = false;
	isSS2 = false;
	return originalCreateSonicContext_r(Sonk, a2);
}

//updated
void SuperSonic::InitSS2()
{
	WRITE_NOP(0x14B543915, 0x2); //force pac file to always load
}

void SuperSonic::Init() 
{
	SuperSonic:InitSS2();
	WRITE_NOP(sigIsNotCyberSpace(), 0x2); //force Super Sonic visual to be loaded in cyberspace (fix crash)

	INSTALL_HOOK(SuperSonicEffectAura_r); //used to delete super aura when detransform
	INSTALL_HOOK(ChangeStateParameter_r);

	INSTALL_HOOK(titanfightCheck_r);
	INSTALL_HOOK(CreateSonicContext_r);
	
	if (!isPerfectParry) //we hook the functions that set anims to force SS2 parry anim, we don't need to do it if perfect parry is enabled since the game will do it for us.
	{
		INSTALL_HOOK(SetNextAnim_r);
		INSTALL_HOOK(SetNextAnim2_r);
		INSTALL_HOOK(SetNextAnim3_r);
	}

	//INSTALL_HOOK(GetNextCombatID);

}