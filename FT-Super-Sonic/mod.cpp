#include "pch.h"
#include "ss.h"


extern "C" {

	__declspec(dllexport) void Init()
	{
		if (!sigValid)
		{
			//MessageBox(nullptr, TEXT("Unsupported game version.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
			//return;
		}

		/**WRITE_JUMP(0x14A8DDAF0, 0x14079D6A0);
		WRITE_JUMP(0x14BBEE270, 0x14089FA50);
		WRITE_JUMP(0x14BBE7D30, 0x14089FA10);*/
		BgmService();
		initConfig();
		init_SuperSonicHacks();
		//init_Util();
		//init_Audio();
        //WRITE_MEMORY(0x000000, uint8_t, 0x2)//
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		SuperSonic_OnFrames(sonicContextPtr);

	}
}