#include "pch.h"
#include "ss.h"
#include "music.h"


extern "C" {

	__declspec(dllexport) void Init()
	{
		if (!sigValid)
		{
			//MessageBox(nullptr, TEXT("Unsupported game version.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
			//return;
		}

		Init_MusicHacks();
		initConfig();
		init_SuperSonicHacks();
		//init_Util();
		//init_Audio();
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		SuperSonic_OnFrames(sonicContextPtr);

	}
}