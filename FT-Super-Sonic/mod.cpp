#include "pch.h"
#include "ss.h"
#include "music.h"
#include "input.h"
#include "./Loaders/LostCodeLoader.h"


std::string modPath = "";

extern "C" {

	__declspec(dllexport) void Init(ModInfo* info)
	{
		if (!sigValid)
		{
			MessageBox(nullptr, TEXT("Unsupported game version.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
			return;
		}

		if (info)
		{
			modPath = info->CurrentMod->Path;

			size_t pos = modPath.find_last_of("\\/");
			if (pos != std::string::npos)
				modPath.erase(pos + 1);
		}
			
		Init_Config(); //get player config
		SuperSonic::Init();
		Init_Music();
		Init_Util(); //miscellaneous stuff to get current island, play state etc...
	}

	__declspec(dllexport) void OnFrame()
	{
		SuperSonic::OnFrames(sonicContextPtr);
	}
}