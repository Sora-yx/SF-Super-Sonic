#include "pch.h"
#include "ss.h"
#include "music.h"
#include "input.h"
#include "../Dependencies/ModInfo.h"
#include "bass.h"
#include <fstream>

time_t t;
std::string modPath = "";
static bool IsGameStarted = false;
bool hedgeMayCry = false;

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
		else
		{
			PrintInfo("Failed to get mod path, music won't work.\n");
		}

		srand((unsigned)time(&t));
			
		Init_Config(); //get player config
		SuperSonic::Init();
		Init_Music();
		Init_Util(); //miscellaneous stuff to get current island, play state etc...

		//WRITE_NOP(0x140967E03, 0x5);
	}

	__declspec(dllexport) void OnFrame()
	{
		SuperSonic::OnFrames();
	}

	//On Exit seems to be called twice; once on startup and once on actual exit
	//we make it so our code only run on the second call.
	__declspec(dllexport) void OnExit()
	{
		if (!IsGameStarted)
		{
			IsGameStarted = true;
			return;
		}

		FreeBass();
	}
}