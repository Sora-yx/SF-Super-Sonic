#include "pch.h"

size_t* SoundDirector = nullptr;
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

static bool transfoSS = false;
const char* backupBGMInfo[2]{ "", 0};

HOOK(void, __fastcall, PlayBGM_r, sigPlayBGM(), size_t* in_soundDirector, __int64 a2, const char** bgmName)
{
	if (!transfoSS)
	{
		backupBGMName = bgmName[0];
		*backupBGMInfo = *bgmName;
	}

	SoundDirector = in_soundDirector;
	originalPlayBGM_r(in_soundDirector, a2, bgmName);
	transfoSS = false;
}

void PlayMusic()
{
	const char** t = backupBGMInfo;

	if (SoundDirector && useSSMusic)
	{
		std::string song = getSSSong();
		t[0] = song.c_str();
		t[1] = backupBGMInfo[1];
		transfoSS = true;
		playBGM(SoundDirector, 0, t);
	}
}

void RestoreOriginalMusic()
{
	const char** bgmInfo = backupBGMInfo;
	if (SoundDirector && useSSMusic)
	{
		bgmInfo[0] = backupBGMName.c_str();
		bgmInfo[1] = backupBGMInfo[1];
		playBGM(SoundDirector, 0, bgmInfo);
	}
}

void Init_Music()
{
	if (useSSMusic)
		INSTALL_HOOK(PlayBGM_r);
}