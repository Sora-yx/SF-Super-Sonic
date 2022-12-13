#include "pch.h"
#include "ss.h"

extern "C" {

	__declspec(dllexport) void Init()
	{
		if (!sigValid)
		{
			MessageBox(nullptr, TEXT("Unsupported game version.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
			return;
		}

		initConfig();
		init_SuperSonicHacks();
		init_Util();
        //WRITE_MEMORY(0x000000, uint8_t, 0x2)//
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		SuperSonic_OnFrames(sonicContextPtr);
	}
}