#include "pch.h"
#include "ss.h"
#include "music.h"


extern "C" {

	__declspec(dllexport) void Init()
	{
		if (!sigValid)
		{
			MessageBox(nullptr, TEXT("Unsupported game version.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
			return;
		}

		initConfig(); //get player config
		init_SuperSonicHacks(); //series of hacks to allow Sonk to turn to Super
		Init_MusicHacks();
		init_Util(); //miscellaneous stuff to get current island, play state etc...
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		SuperSonic_OnFrames(sonicContextPtr);
	}
}