#pragma once

struct Sonic
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
};

struct __declspec(align(16)) BlackboardStatus
{
	__int64 field[5];
	unsigned int SuperSonic;
	__int64 field2[42];
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
	__int64 pGOCPlayerHsm;
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

struct __declspec(align(4)) StatePluginBossBattle
{
	int field_10;
	int field_14;
	int field_18;
	int field_22;
	int field_26;
	int field_30;
	int field_34;
	int field_38;
	SonicContext* pSonicContext;
	int field_50;
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



FUNCTION_PTR(size_t, __fastcall, sub_140b8a8d0, 0x140b8a8d0, size_t* a1);


FUNCTION_PTR(__int64, __fastcall, sub_14086C710, 0x14086C710, SonicContext* a1, __int64 a2);

FUNCTION_PTR(void, __fastcall, sub_14079C720, 0x14079C720, __int64 a1, __int64 a2);
FUNCTION_PTR(char, __fastcall, sub_1407EFB20, 0x1407EFB20, SonicContext* SonicCOntext, __int64 a2, __int64 a3);