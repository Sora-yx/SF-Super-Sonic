#pragma once

struct __declspec(align(8)) GameModeStagePlay
{
	int vftable;
	int idk2;
	GameModeStagePlay* instance;
	char a[20];
	char b[12];
	char c[8];
};


struct __declspec(align(0x10)) Sonic
{
	void* vfTable;
	char gap2[70];
	char field_48;
	char gap49[15];
	char pComponents;
	char gap59[4];
	char field_5D;
	__declspec(align(4)) char field_60;
	char gap61[6];
	char field_67[5];
	char field_6C[100];
	char field_7c[541];
};

struct __declspec(align(0x10)) BlackboardStatus
{
	INSERT_PADDING(0x10);
	int64_t StateFlags;
	int64_t WorldFlags;
	INSERT_PADDING(0xF0);
	bool IsIslandSideView;
	INSERT_PADDING(0x4F);
};

struct __declspec(align(0x10)) GOCPlayerHsm
{
	uint8_t fieldA[272];
	__int64 stateID;
	__int64 stateMode;
	uint8_t fieldB[4];
};

struct SonicContext
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
	__int64 field_18;
	Sonic* pSonic;
	BlackboardStatus* pBlackBoardStatus;
	__int64 field_30;
	GOCPlayerHsm* pGOCPlayerHsm;
	__int64 field_40;
	__int64 pGOCPlayerKinematicPrams;
	__int64 pGOCPlayerBlackboard;
	__int64 field_58;
	__int64 field_60;
	__int64 field_68;
	__int64 field_70;
	__int64 field_78;
	__int64 field_80;
};



struct GOCKinematicPrams
{
	__int16 field_0;
	char gap2[70];
	char field_48;
	char gap49[15];
	char pComponents;
	char gap59[4];
	char field_5D;
	__declspec(align(4)) char field_60;
	char gap61[6];
	char field_67[5];
	char field_6C[100];
	int idk;
	float spdY;
	float spd3;
	char field_7c[541];
};

struct SSEffAuraS
{
	void* PEffectVfTable;
	__int64 field0[2];
	Sonic* Sonk;
	__int64 field1[4];
	__int64 AuraFlagMaybe;
	__int64 field2[60];
};


struct Message
{
	__int64 field;
	__int64 msgID;
	__int64 field2[20];
};


struct StatePlay
{
	void* GameModeStatePlay;
	int field_0[19];
};

struct CyberStatePlay
{
	void* CyberStatePlay;
	int field_0[15];
};

struct __declspec(align(0x8)) SoundDirector
{
	INSERT_PADDING(672);
};

static FUNCTION_PTR(bool, __fastcall, IsSuperSonic, sigIsSuperSonic(), SonicContext* a1);
static FUNCTION_PTR(void, __fastcall, SubRing, sigSubRings(), SonicContext* sContext, int count);
static FUNCTION_PTR(void, __fastcall, playBGM, sigPlayBGM(), SoundDirector* a1, const char** a3);



static FUNCTION_PTR(char, __fastcall, TriggerSuperSonic, sigTriggerSS(), SonicContext* a1, bool enabled);
static FUNCTION_PTR(size_t, __fastcall, GetRings, sigGetRings(), SonicContext* sContext);
static FUNCTION_PTR(char, __fastcall, SetSonicFall, sigSetSonicFall(), SonicContext* a1, char a2);
static FUNCTION_PTR(char, __fastcall, ChangeStateParameter, 0x1408033E0, SonicContext* Sonk, __int64 actionID, __int64 a3);
static FUNCTION_PTR(__int64, __fastcall, SSAuraDestructor, sigsub_SSAuraDestructor(), SSEffAuraS* a1);
static FUNCTION_PTR(__int64, __fastcall, SetNewMSG, sigSetNewMsg(), __int64* a1, int a2);