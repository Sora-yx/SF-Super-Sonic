#include "pch.h"
#include "xinput.h"

bool nolimit = false;
int AscendBtn = XINPUT_GAMEPAD_A;
int DescendBtn = XINPUT_GAMEPAD_B;
int TransformBtn = XINPUT_GAMEPAD_DPAD_UP;
int UntransformBtn = XINPUT_GAMEPAD_DPAD_DOWN;

std::string AscendKey = "SPACE";
std::string DescendKey = "CTRL";
std::string TransformKey = "Y";
std::string UntransformKey = "Z";
uint8_t useSSMusic = TRUE;

int currentSuperFormIndex = random;

float vol = 80.0f;

void Init_Config()
{
	const INIReader reader("config.ini");

	if (reader.ParseError() != 0)
	{
		MessageBox(nullptr, TEXT("Cannot load configuration file, mod settings won't work.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
		return;
	}

	nolimit = reader.GetBoolean("Mod", "nolimit", nolimit);

	useSSMusic = (uint8_t)reader.GetBoolean("Audio", "useSSMusic", useSSMusic);
	vol = reader.GetFloat("Audio", "vol", vol);
	currentSuperFormIndex = reader.GetInteger("Audio", "musicList", currentSuperFormIndex);

	//input
	AscendBtn = reader.GetInteger("Input", "AscendBtn", AscendBtn);
	DescendBtn = reader.GetInteger("Input", "DescendBtn", DescendBtn);
	TransformBtn = reader.GetInteger("Input", "TransformBtn", TransformBtn);
	UntransformBtn = reader.GetInteger("Input", "UntransformBtn", UntransformBtn);

	//keyboard
	AscendKey = reader.Get("Key", "AscendKey", AscendKey);
	DescendKey = reader.Get("Key", "DescendKey", DescendKey);
	TransformKey = reader.Get("Key", "TransformKey", TransformKey);
	UntransformKey = reader.Get("Key", "UntransformKey", UntransformKey);	

	if (!vol)
		vol = -10000.0f;

	vol = 100.0f * (89.0f * vol / 100.0f - 100.0f);
}