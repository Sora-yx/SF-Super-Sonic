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

SonicContext* sonicContextPtr = nullptr;

bool isSuper = false;
bool transfoAllowed = false;

void Untransfo(SonicContext* SContext)
{
	TriggerSuperSonic(SContext, false);
	isSuper = false;
	SetSonicFall(SContext, 0);
	/**auto pSonic = GetPSonic(SContext);
	auto v10 = sub_1407A8490();
	auto v11 = sub_140B8B120(pSonic, (__int64)v10);
	SetNextAnim(v11, "FALL", 0xFEu);*/
}

FUNCTION_PTR(__int64, __fastcall, sub_140817880, 0x140817880, SonicContext* a1);
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

void RemoveRings(SonicContext* SContext)
{
	SubRing(SContext, 1);
}

void changeSSMusic();
void Transfo_CheckInput(SonicContext* SContext)
{
	auto ring = GetRings(SContext);

	if (GetKeyState('S') & 0x8000 && !isSuper && (nolimit || ring >= 50)) 
	{
		if (transfoAllowed)
		{
			//changeSSMusic();
			TriggerSuperSonic(SContext, true);
			isSuper = true;
			return;
		}
	}

	if (GetKeyState('Z') & 0x8000 && isSuper) //detransfo
	{
		Untransfo(SContext);
		return;
	}
}

HOOK(void, __fastcall, SetNextAnimMaybe_r, 0x1407A7710, __int64 a1, const char* action, unsigned __int8 a3)
{
	std::string toto(action);

	if (toto == "STAND" || toto == "RUNNING")
	{
		transfoAllowed = true;
	}
	else
	{
		transfoAllowed = false;
	}

	originalSetNextAnimMaybe_r(a1, action, a3);
}

//we hack the function that manage MSG for Sonic as we need the SonicContext instance to call Super Sonic
HOOK(char, __fastcall, PlayerStateProcessMSG_r, sigPStateProcessMSG(), SonicContext* SContext, __int64 a2, __int64 a3)
{
	size_t curAction = *(DWORD*)(a2 + 8);

	sonicContextPtr = SContext;

	Transfo_CheckInput(SContext);

	return originalPlayerStateProcessMSG_r(SContext, a2, a3);
}

struct _declspec(align(8)) StateFly
{
	char field_10[388];
};

struct Message
{
	__int64 field;
	__int64 msgID;
	__int64 field2[20];
};

int prevAction = -1;
int curAction = -1;

void printAction()
{
	if (prevAction != curAction)
	{
		PrintInfo("cur msgID: %d", curAction);
		prevAction = curAction;
	}
}

HOOK(char, __fastcall, sub_1407F1260, 0x1407F1260, SonicContext* Sonk, Message* a2)
{
	curAction = a2->msgID;
	return originalsub_1407F1260(Sonk, a2);
}

HOOK(__int64, __fastcall, SetNewAction, 0x140798A50, __int64 a1, __int64 a2, __int64 a3)
{
	PrintInfo("Set New Action: %d\n", a2);
	return originalSetNewAction(a1, a2, a3);
}

HOOK(StateFly*, __fastcall, PlayerStateFlyCOnstructor, 0x140821690, __int64 a1)
{
	return originalPlayerStateFlyCOnstructor(a1);
}

HOOK(char, __fastcall, SetSuperSonicNextAction, 0x14086EE40, SonicContext* a1, char a2, unsigned int a3)
{

	return originalSetSuperSonicNextAction(a1, a2, a3);
}

HOOK(bool, __fastcall, isSuperSonic, 0x140798FF0, SonicContext* pSonic)
{
	return originalisSuperSonic(pSonic);
}

HOOK(void, __fastcall, SetAura_r, 0x14078EFD0, __int64 a1, bool enabled)
{
	enabled = false;
	return originalSetAura_r(a1, enabled);
}

void init_SuperSonicHacks()
{
	//WRITE_MEMORY(0x140821AF0, uint8_t, 0xC3);
	INSTALL_HOOK(PlayerStateProcessMSG_r);
	INSTALL_HOOK(SetNextAnimMaybe_r);
	INSTALL_HOOK(SetNewAction);
	//INSTALL_HOOK(PlayerStateFlyCOnstructor);
	//WRITE_JUMP(0x140821690, 0x14B6F9040);
	INSTALL_HOOK(SetSuperSonicNextAction);
	//WRITE_MEMORY(0x14086EE40, int, 0xC3);

	INSTALL_HOOK(isSuperSonic);
	//INSTALL_HOOK(SetAura_r);
	WRITE_MEMORY(0x14BEB6520, int, 0xC3);
	//WRITE_JUMP(0x14086EEAB, 0x14086EE97);
	//WRITE_JUMP(0x14082C13F, 0x14082C160);

	/**WRITE_NOP(0x0000000140864ED5, 5);
	WRITE_NOP(0x0000000140864EE3, 5);*/


}