#include "pch.h"



FUNCTION_PTR(void, __fastcall, PlayMusicMaybe, sigPlayMusic(), size_t* a1, size_t* a2);
FUNCTION_PTR(void*, __cdecl, sub_14088FB90, 0x14088FB90);
FUNCTION_PTR(size_t*, __fastcall, AllocateSomething, 0x140172350, size_t* a1, void* a2);

struct Music
{
	const char* name;
	__int64 idk;
	char toto;
	char toto2;
	char toto3;
	__int64 idk2;
	__int64 idk3;
	__int64 idk4;
	char toto4;
	char toto5;
	char toto6;
	__int64 idk6;
	__int64 idk7;
};


size_t* zicPtr = nullptr;
Music* zic = nullptr;

HOOK(void, __fastcall, PlayMusicMaybe_r, 0x14088DDB0, size_t* a1, Music* a2)
{
	zicPtr = a1;
	zic = a2;
	originalPlayMusicMaybe_r(a1, a2);
}

inline static void* xmmword_14120A380 = (void*)(0x14120A380);

void changeSSMusic()
{

	//zic->name = "bgm_sys_title";
	//PlayMusicMaybe(zicPtr, (size_t*)zic);
}

void init_Audio()
{
	INSTALL_HOOK(PlayMusicMaybe_r);
}