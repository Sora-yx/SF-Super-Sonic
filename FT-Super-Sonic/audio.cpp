#include "pch.h"

SoundDirector* SoundDirectorPtr = nullptr;
std::string backupBGMName = "";
extern int currentIsland;

std::string songArray[] =
{
	"bgm_boss01_phase02",
	"bgm_boss02_phase02",
	"bgm_boss03_phase02",
	"bgm_maintheme"
};

std::string getSSSong()
{
	return currentIsland < 4 ? songArray[currentIsland] : songArray[3];
}

static bool disabled = false;

HOOK(void, __fastcall, PlayBGM_r, sigPlayBGM(), SoundDirector* in_soundDirector, const char** bgmName)
{
	if (!disabled) 
	{
		backupBGMName = bgmName[0];
	}

	originalPlayBGM_r(in_soundDirector, bgmName);
	disabled = false;
}

void PlayMusic()
{
	std::string son = getSSSong();
	const char* bgmInfo = son.c_str();
	const char** song = &bgmInfo;

	if (SoundDirectorPtr && useSSMusic)
	{
		disabled = true;
		playBGM(SoundDirectorPtr, song);
	}
}

//unsupported sig scan :(
HOOK(SoundDirector*, __fastcall, SoundDirector_Constructor_r, 0x140922C40, SoundDirector* a1)
{
	SoundDirectorPtr = originalSoundDirector_Constructor_r(a1);
	return SoundDirectorPtr;
}

//seems to only work from time to time, lol
void RestoreOriginalMusic()
{
	const char* bgmInfo = backupBGMName.c_str();
	const char** song = &bgmInfo;
	if (SoundDirectorPtr && useSSMusic)
	{
		playBGM(SoundDirectorPtr, song);
	}
}

void Init_Music()
{
	if (useSSMusic)
	{
		INSTALL_HOOK(PlayBGM_r); 
		INSTALL_HOOK(SoundDirector_Constructor_r);
	}
}