#include "pch.h"
#include "ss.h"

extern "C" {

	__declspec(dllexport) void Init()
	{
        const INIReader reader("config.ini");

        if (reader.ParseError() != 0)
            PrintInfo(("Cannot load configuration file, mod won't work.\n"));

		init_SuperSonicHacks();
        //WRITE_MEMORY(0x000000, uint8_t, 0x2)//
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{

	}
}