#include "pch.h"

bool nolimit = false;

void initConfig()
{
	const INIReader reader("config.ini");

    if (reader.ParseError() != 0)
    {
        MessageBox(nullptr, TEXT("Cannot load configuration file, mod settings won't work.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
        return;
    }

    nolimit = reader.GetBoolean("Mod", "nolimit", nolimit);
    nolimit = nolimit;

}