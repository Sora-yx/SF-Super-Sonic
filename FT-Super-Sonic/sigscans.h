#pragma once

SIG_SCAN
(
	sigPStateProcessMSG,
	0x1407F3590,
	"\x40\x55\x53\x56\x57\x41\x56\x41\x57\x48\x8D\x6C\x24\xB8", 
	"xxxxxxxxxxxxxx"
)

SIG_SCAN
(
	sigTriggerSS,
	0x140864E60,
	"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x0F\xB6\xDA\x48\x8B\xF9\xE8\xCC\xCC\xCC\xCC\x3A", 
	"xxxxxxxxxxxxxxxxx????x"
)

SIG_SCAN
(
	sigPlayMusic,
	0x14088DDB0,
	"\xE8\xCC\xCC\xCC\xCC\x44\x8B\x6D\x67", 
	"x????xxxx"
)

SIG_SCAN
(
	sigGetRings,
	0x140798CE0,
	"\x48\x8B\x41\x30\x8B\x40\x28", 
	"xxxxxxx"
)

SIG_SCAN
(
	sigSubRings,
	0x140799740,
	"\x4C\x8B\x49\x30\x44", 
	"xxxxx"
)

SIG_SCAN
(
	sigSetSonicFall,
	0x14086EC50,
	"\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\x59", 
	"xxxxxxxxxxxxxxxxxxxxxxx"
)

SIG_SCAN
(
	sigChangeStateParameter,
	0x140798A50,
	"\xE8\xCC\xCC\xCC\xCC\x41\xF6\x46\x48\x02", 
	"x????xxxxx"
)

SIG_SCAN
(
	sigIsSuperSonic,
	0x140798FF0,
	"\x48\x83\xEC\x28\x48\x8B\x49\x28\xE8\xCC\xCC\xCC\xCC\x83", 
	"xxxxxxxxx????x"
)