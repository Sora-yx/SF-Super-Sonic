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

SIG_SCAN
(
	m_SigSetIslandBgm,
	0x14BF41210,
	"\x48\x85\xD2\x0F\x84\x46\x01\x00\x00\x48\x89\xE0", 
	"xxxxxxxxxxxx"
);

app::snd::BgmIdExtension* in_pBgmIdExtension_;
int64_t a2_ = 0;
const char** out_ppMusicId_;

FUNCTION_PTR(void, __fastcall, SetIsladnBGM, 0x1408BB680, app::snd::BgmIdExtension* in_pBgmIdExtension, int64_t a2, const char** out_ppMusicId);

bool enabled = false;
HOOK(void, __fastcall, SetIslandBgm_r, m_SigSetIslandBgm(), app::snd::BgmIdExtension* in_pBgmIdExtension, int64_t a2, const char** out_ppMusicId)
{
	originalSetIslandBgm_r(in_pBgmIdExtension, a2, out_ppMusicId);

	if (enabled)
	{
		in_pBgmIdExtension->TrackIndex = 4;
		*out_ppMusicId = "bgm_maintheme";
		enabled = false;
	}

	in_pBgmIdExtension_ = in_pBgmIdExtension;
	a2_ = a2;
	out_ppMusicId_ = out_ppMusicId;
}

void PlayMusic()
{
	enabled = true;
	SetIsladnBGM(in_pBgmIdExtension_, a2_, out_ppMusicId_);
}

void BgmService()
{
	INSTALL_HOOK(SetIslandBgm_r);
}