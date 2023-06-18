#pragma once

SIG_SCAN
(
	sigPStateProcessMSG,
	0x140816E00,
	"\x40\x55\x53\x56\x57\x41\x56\x41\x57\x48\x8D\x6C\x24\xB8", 
	"xxxxxxxxxxxxxx"
)

SIG_SCAN
(
	sigTriggerSS,
	0x140889B10,
	"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x0F\xB6\xDA\x48\x8B\xF9\xE8\xCC\xCC\xCC\xCC\x3A",
	"xxxxxxxxxxxxxxxxx????x"
)


SIG_SCAN
(
	sigGetRings,
	0x1407BAAB0,
	"\x48\x8B\x41\x30\x8B\x40\x28", 
	"xxxxxxx"
)

SIG_SCAN
(
	sigSubRings,
	0x1407BB510,
	"\x4C\x8B\x49\x30\x44", 
	"xxxxx"
)

SIG_SCAN
(
	sigSetSonicFall,
	0x140893BB0,
	"\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\x59", 
	"xxxxxxxxxxxxxxxxxxxxxxx"
)

SIG_SCAN
(
	sigChangeStateParameter,
	0x1407BA820,
	"\xE8\xCC\xCC\xCC\xCC\x41\xF6\x46\x48\x02", 
	"x????xxxxx"
)

SIG_SCAN
(
	sigIsSuperSonic,
	0x1407BADC0,
	"\x48\x83\xEC\x28\x48\x8B\x49\x28\xE8\xCC\xCC\xCC\xCC\x83", 
	"xxxxxxxxx????x"
)

SIG_SCAN
(
	sigIsNotCyberSpace,
	0x140223070,
	"\xF6\x81\xB8\x00\x00\x00\x2E", 
	"xxxxxxx"
)

SIG_SCAN
(
	sigEndPlayStateMaybe,
	0x147A7BC80,
	"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x40\x48\x8B\x0D", 
	"xxxxxxxxxxxxx"
)


SIG_SCAN
(
	sigsub_SSEFfectAura,
	0x14A9A8100,
	"\x40\x53\x48\x83\xEC\x20\x48\x89\xCB\xE8\xCC\xCC\xCC\xCC\x48\x8D\x05\xCC\xCC\xCC\xCC\x48\x8D\x4B\x48", 
	"xxxxxxxxxx????xxx????xxxx"
)

SIG_SCAN
(
	sigsub_SSAuraDestructor,
	0x1407A5CB0,
	"\x48\x83\xEC\x28\x48\x89\x7C\x24\x20", 
	"xxxxxxxxx"
)

SIG_SCAN
(
	sigsub_IsNotInCyber,
	0x14079BB8B,
	"\x74\x12\x41\xB0\x02", 
	"xxxxx"
)


SIG_SCAN
(
	sigPlayBGM,
	0x14BDBB3C0,
	"\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x57\x48\x83\xEC\x50\x48\x63",
	"xxxxxxxxxxxxxxxxx"
);

SIG_SCAN
(
	sigEndGameStatePlay,
	0x147A7BC80,
	"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x40\x48\x8B\x0D", 
	"xxxxxxxxxxxxx"
)

SIG_SCAN
(
	sigSetNewMsg,
	0x15267AA20,
	"\x48\x8D\x05\xCC\xCC\xCC\xCC\x89\x51\x08\x48\x89\x01\x31", 
	"xxx????xxxxxxx"
)

SIG_SCAN
(
	sigBattleRushNextPhase,
	0x14017ECF0,
	"\x40\x53\x48\x83\xEC\x40\x48\x8B\x0D\xCC\xCC\xCC\xCC\x48\x8B\xDA\xE8", 
	"xxxxxxxxx????xxxx"
)