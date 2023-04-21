#include "pch.h"

namespace app::snd
{
	class BgmIdExtension
	{
	public:
		INSERT_PADDING(0x9C);
		int TrackIndex;
		INSERT_PADDING(0x30);

		virtual ~BgmIdExtension() = default;
	};
}

size_t* SoundDirector = nullptr;
const char** SSMusic = nullptr;
const char** test = nullptr;
std::string testS = "";
size_t backupMusic;

SIG_SCAN
(
	m_SigIslandMusicPlayer,
	0x1408BA270,
	"\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\xD0\x48\x81\xEC\x30\x01\x00\x00\x48\x8B\xD9", 
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
);

SIG_SCAN
(
	sigPlayBGM,
	0x14BDBB3C0,
	"\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x57\x48\x83\xEC\x50\x48\x63", 
	"xxxxxxxxxxxxxxxxx"
);

FUNCTION_PTR(void, __fastcall, playBGM, sigPlayBGM(), size_t* a1, size_t a2, const char** a3);
static bool transfoSS = false;

HOOK(size_t*, __fastcall, IslandMusicPlayer_r, m_SigIslandMusicPlayer(), size_t* a1)
{
	a1 = originalIslandMusicPlayer_r(a1);
	return a1;
}

HOOK(void, __fastcall, PlayBGM_r, sigPlayBGM(), size_t* in_soundDirector, size_t a2, const char** in_bgmName)
{
	if (!transfoSS)
	{
		backupMusic = (size_t)in_bgmName;
		testS = in_bgmName[0];
	}
	SoundDirector = in_soundDirector;
	originalPlayBGM_r(in_soundDirector, a2, in_bgmName);
	transfoSS = false;
}

void PlayMusic()
{
	if (SoundDirector && backupMusic)
	{
		transfoSS = true;
		SSMusic = (const char**)backupMusic;
		SSMusic[0] = "bgm_boss01_phase02";
		playBGM(SoundDirector, 0, SSMusic);
	}
}

void RestoreOriginalMusic()
{
	if (SoundDirector && backupMusic)
	{
		test = (const char**)backupMusic;
		test[0] = testS.c_str();
		playBGM(SoundDirector, 0, (const char**)test);
	}
}

void Init_MusicHacks()
{
	INSTALL_HOOK(IslandMusicPlayer_r);
	INSTALL_HOOK(PlayBGM_r);
}