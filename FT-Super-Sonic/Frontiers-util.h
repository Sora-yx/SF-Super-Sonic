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

typedef struct SonicContext;
struct __declspec(align(0x10)) Sonic
{
	INSERT_PADDING(0x2E0);
	SonicContext* sonkContext;
};

struct __declspec(align(0x10)) Blackboardstatus
{
	INSERT_PADDING(56);
	int64_t StateFlags;
	int64_t WorldFlags;
	INSERT_PADDING(240);
	bool IsIslandSideView;
	INSERT_PADDING(71);
};

struct __declspec(align(0x10)) GOCPlayerHsm
{
	uint8_t fieldA[272];
	__int64 stateID;
	__int64 stateMode;
	uint8_t fieldB[4];
};

struct ScriptSequenceManager
{
	char a[0x70];
	void* gameStepListener;
	void* saveManagerListener;

};

struct SonicContext
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
	__int64 field_18;
	Sonic* pSonic;
	Blackboardstatus* pBlackBoardStatus;
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
static FUNCTION_PTR(char, __fastcall, ChangeStateParameter, 0x1408ABE70, SonicContext* Sonk, __int64 actionID, __int64 a3); 
static FUNCTION_PTR(__int64, __fastcall, SSAuraDestructor, sigsub_SSAuraDestructor(), SSEffAuraS* a1);
static FUNCTION_PTR(__int64, __fastcall, SetNewMSG, sigSetNewMsg(), __int64* a1, int a2);

static FUNCTION_PTR(__int64, __fastcall, SceneParametersGetSceneData, 0x1402484E0, __int64* a1);
static FUNCTION_PTR(void*, __cdecl, GOCPlayerVisualGetStaticClass, 0x1408BC3E0); 
static FUNCTION_PTR(__int64, __fastcall, GameObjectGetGoc, 0x140D35EB0, __int64 a1, __int64 a2); 
static FUNCTION_PTR(__int64, __fastcall, GOCPlayerVisualGetVisualByHashName, 0x1408BB880, __int64 a1, int a2); 
static FUNCTION_PTR(__int64, __fastcall, VisualChangeToSuperSonic2, 0x1409DF740, __int64 a1);

static FUNCTION_PTR(__int64, __fastcall, VisualChangeToSuperSonic, 0x1409DF8D0, __int64 a1);  
//static FUNCTION_PTR(void, __fastcall, sub_1408AB5A0, 0x1408AB5A0, __int64 a1, __int64 a2, __int64 a3);
static FUNCTION_PTR(__int64, __fastcall, SetCombatFlags, 0x1408AC8E0, __int64* a1, __int64 a2, __int64 a3);
static FUNCTION_PTR(__int64, __fastcall, SetWorldFlags, 0x1409CDA90, __int64* a1, __int64 a2, __int64 a3);

static FUNCTION_PTR(void, __fastcall, DisableCombatFlags, 0x1409CB810, __int64* a1, __int64 a2); 
static FUNCTION_PTR(signed __int64, __fastcall, DisableWorldFlags, 0x1409CC060, __int64* a1, __int64 a2);

#define DataArray(type, name, address, length) \
    static type *const name = (type *)address; static const int name##_Length = length

DataArray(char, pVisualSuperSonicHash, 0x143CDCC68, 5);