#include "pch.h"
#include "ss.h"
#include "music.h"


extern "C" {

	__declspec(dllexport) void Init()
	{
		if (!sigValid)
		{
			MessageBox(nullptr, TEXT("Unsupported game version.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
			//return;
		}

		initConfig();
		init_SuperSonicHacks();
		Init_MusicHacks();
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		SuperSonic_OnFrames(sonicContextPtr);

	}
}