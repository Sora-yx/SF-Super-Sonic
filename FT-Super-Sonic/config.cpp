#include "pch.h"
#include "xinput.h"


bool nolimit = false;
int AscendBtn = XINPUT_GAMEPAD_A;
int DescendBtn = XINPUT_GAMEPAD_B;
int TransformBtn = XINPUT_GAMEPAD_DPAD_UP;
int UntransformBtn = XINPUT_GAMEPAD_DPAD_DOWN;

char AscendKey = 'W';
char DescendKey = 'X';
char TransformKey = 'Y';
char UntransformKey = 'Z';
uint8_t useSSMusic = TRUE;

void initConfig()
{
	const INIReader reader("config.ini");

	if (reader.ParseError() != 0)
	{
		MessageBox(nullptr, TEXT("Cannot load configuration file, mod settings won't work.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
		return;
	}

	nolimit = reader.GetBoolean("Mod", "nolimit", nolimit);
	useSSMusic = (uint8_t)reader.GetBoolean("Mod", "useSSMusic", useSSMusic);
	//input
	AscendBtn = reader.GetInteger("Input", "AscendBtn", AscendBtn);
	DescendBtn = reader.GetInteger("Input", "DescendBtn", DescendBtn);
	TransformBtn = reader.GetInteger("Input", "TransformBtn", TransformBtn);
	UntransformBtn = reader.GetInteger("Input", "UntransformBtn", UntransformBtn);

	std::string a = "";
	//keyboard
	AscendKey = *reader.Get("Key", "AscendKey", a).data();
	std::string d = "";
	DescendKey = *reader.Get("Key", "DescendKey", d).data();
	std::string t = "";
	TransformKey = *reader.Get("Key", "TransformKey", t).data();
	std::string u = "";
	UntransformKey = *reader.Get("Key", "UntransformKey", u).data();
}