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

struct SonicContext
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
	__int64 field_18;
	Sonic* pSonic;
	__int64 pBlackBoardStatus;
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



FUNCTION_PTR(size_t, __fastcall, sub_140b8a8d0, 0x140b8a8d0, size_t* a1);
