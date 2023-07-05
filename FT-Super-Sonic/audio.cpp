#include "pch.h"
#include "bass_vgmstream.h"
#include <memory>

SoundDirector* SoundDirectorPtr = nullptr;
std::string backupBGMName = "";
extern int currentIsland;
static  bool bassinit = false;

//they need to be global, don't ask
const char* restoreTrack = "";
const char* s = "DISABLE";

std::string songArray[] =
{
	"sonic2",
	"sonic3",
	"s&k",
	"mania",
	"sa1",
	"sa2",
	"heroes",
	"undefeatable",
	"wyvern_ext",
	"knight",
	"maintheme"
};

std::string getSSSong()
{
	if (currentSuperFormIndex == random)
	{
		std::string s = songArray[rand() % LengthOfArray(songArray)];
		return s;
	}

	return songArray[currentSuperFormIndex];
}

static DWORD basschan = 0;

#pragma region Bass Library Functions

static void __stdcall onTrackEnd(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	BASS_ChannelStop(channel);
	BASS_StreamFree(channel);
}

void ResumeBassMusic()
{
	if (!bassinit || !basschan)
		return;

	BASS_ChannelPlay(basschan, false);
}

void __cdecl PauseBassMusic()
{
	if (!bassinit || !basschan)
		return;

	BASS_ChannelPause(basschan);
}

void StopBassMusic()
{
	if (!bassinit || !basschan)
		return;

	BASS_ChannelStop(basschan);
	BASS_StreamFree(basschan);
}

void PlayBassMusic(const char* filename)
{
	if (!bassinit)
		return;

	if (basschan != 0)
	{
		BASS_ChannelStop(basschan);
		BASS_StreamFree(basschan);
	}

	basschan = BASS_VGMSTREAM_StreamCreate(filename, BASS_SAMPLE_LOOP);

	if (basschan != 0)
	{
		// Stream opened!
		BASS_ChannelPlay(basschan, false);
		BASS_ChannelSetAttribute(basschan, BASS_ATTRIB_VOL, (vol + 10000) / 30000.0f);
		BASS_ChannelSetSync(basschan, BASS_SYNC_END, 0, onTrackEnd, nullptr);
	}
}

#pragma endregion

static bool disabled = false;

HOOK(void, __fastcall, PlayBGM_r, sigPlayBGM(), SoundDirector* in_soundDirector, const char** bgmName)
{
	if (!disabled)
	{
		StopBassMusic();
		backupBGMName = bgmName[0]; 
	}

	originalPlayBGM_r(in_soundDirector, bgmName);
	disabled = false;
}


void PlayMusic()
{
	if (SoundDirectorPtr && useSSMusic)
	{
		disabled = true;
		playBGM(SoundDirectorPtr, &s); //disable current music
		std::string bgmName = getSSSong();
		std::string folderPath = (std::string)modPath + "music";
		std::string fullPath = findFile(folderPath, bgmName); //get extension of the music and combine the path with it.
		const uint32_t size = fullPath.length() + 1;
		std::unique_ptr<char[]> trackNameChar(new char[size]);
		strcpy_s(trackNameChar.get(), size, fullPath.c_str());
		PlayBassMusic(trackNameChar.get()); //use bass to play music so we can use custom song without replacing anything
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
	const uint32_t size = backupBGMName.length() + 1;
	std::unique_ptr<char[]> trackNameChar(new char[size]);
	strcpy_s(trackNameChar.get(), size, backupBGMName.c_str());

	restoreTrack = trackNameChar.get();

	if (SoundDirectorPtr && useSSMusic)
	{
		StopBassMusic();
		playBGM(SoundDirectorPtr, &restoreTrack);
	}
}

//Those libraries are needed to load the VGMStream version of Bass, which allows a lot more format for music and make loop easier, this is a but overkill for a simple mod, but easy to use so.
const char* BassDlls[] =
{
	"libatrac9.dll",
	"libcelt-0061.dll",
	"libcelt-0110.dll",
	"libg719_decode.dll",
	"libmpg123-0.dll",
	"libspeex-1.dll",
	"libvorbis.dll",
	"avutil-vgmstream-57.dll",
	"avcodec-vgmstream-59.dll",
	"avformat-vgmstream-59.dll",
	"swresample-vgmstream-4.dll",
	"bass_vgmstream.dll"
};

void Init_Music()
{
	if (useSSMusic)
	{
		std::string Path = modPath + "/bass/";
		std::string dll = Path + "bass.dll";

		HMODULE bassDLL = LoadLibraryA(dll.c_str());
		if (bassDLL)
		{
			INSTALL_HOOK(PlayBGM_r);
			INSTALL_HOOK(SoundDirector_Constructor_r);
			bassinit = !!BASS_Init(-1, 44100, BASS_DEVICE_3D, nullptr, nullptr);
			if (bassinit)
			{
				BASS_Set3DFactors(0.1f, 0.1f, 0.0f);
				BASS_SetConfig(BASS_CONFIG_3DALGORITHM, BASS_3DALG_FULL);

				for (int i = 0; i < LengthOfArray(BassDlls); i++)
				{
					dll = Path + BassDlls[i];
					LoadLibraryA(dll.c_str());
				}

				PrintInfo("Bass library loaded!\n");
			}

		}
	}
}