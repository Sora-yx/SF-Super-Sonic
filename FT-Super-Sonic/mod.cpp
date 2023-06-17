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

		Init_Config(); //get player config
		SuperSonic::Init();
		Init_Music();
		Init_Util(); //miscellaneous stuff to get current island, play state etc...
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		SuperSonic::OnFrames(sonicContextPtr);
	}
}