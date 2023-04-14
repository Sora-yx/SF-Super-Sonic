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

void initConfig()
{
	const INIReader reader("config.ini");

	if (reader.ParseError() != 0)
	{
		MessageBox(nullptr, TEXT("Cannot load configuration file, mod settings won't work.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
		return;
	}

	nolimit = reader.GetBoolean("Mod", "nolimit", nolimit);
	//input
	AscendBtn = reader.GetInteger("Input", "AscendBtn", AscendBtn);
	DescendBtn = reader.GetInteger("Input", "DescendBtn", DescendBtn);
	TransformBtn = reader.GetInteger("Input", "TransformBtn", TransformBtn);
	UntransformBtn = reader.GetInteger("Input", "UntransformBtn", UntransformBtn);

	std::string k;
	//keyboard
	AscendKey = *reader.Get("Key", "AscendKey", k).data();
	DescendKey = *reader.Get("Key", "DescendKey", k).data();
	TransformKey = *reader.Get("Key", "TransformKey", k).data();
	UntransformKey = *reader.Get("Key", "UntransformKey", k).data();
}