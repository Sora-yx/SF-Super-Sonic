#include "pch.h"

bool limit = true;

void initConfig()
{
	const INIReader reader("config.ini");

    if (reader.ParseError() != 0)
    {
        MessageBox(nullptr, TEXT("Cannot load configuration file, mod settings won't work.\n"), TEXT("Super Sonic Mod"), MB_ICONERROR);
        return;
    }

    limit = reader.GetBoolean("Mod", "limit", limit);

}