#pragma once


CL_SCAN_SIGNATURE
(
	sigTriggerSS,
	0x1409C4D90,
	"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x0F\xB6\xDA\x48\x8B\xF9\xE8\xCC\xCC\xCC\xCC\x84", 
	"xxxxxxxxxxxxxxxxx????x"
)


CL_SCAN_SIGNATURE
(
	sigGetRings,
	0x1408AC170,
	"\x48\x8B\x41\x30\x8B\x40\x28", 
	"xxxxxxx"
)


CL_SCAN_SIGNATURE
(
	sigSubRings,
	0x1408ACD30,
	"\x4C\x8B\x49\x30\x44", 
	"xxxxx"
)


CL_SCAN_SIGNATURE
(
	sigSetSonicFall,
	0x1409D1140,
	"\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\x59", 
	"xxxxxxxxxxxxxxxxxxxxxxx"
)

//updated
CL_SCAN_SIGNATURE
(
	sigIsSuperSonic,
	0x1408AC4C0,
	"\x48\x83\xEC\x28\x48\x8B\x49\x28\xE8\xCC\xCC\xCC\xCC\x83", 
	"xxxxxxxxx????x"
)


CL_SCAN_SIGNATURE
(
	sigIsNotCyberSpace,
	0x14088B0EB,
	"\x74\x18\x41\xB9\x01\x00\x00\x00\x48\x8D\x15\xCC\xCC\xCC\xCC\x41\xB0\x02", 
	"xxxxxxxxxxx????xxx"
)

//updated
CL_SCAN_SIGNATURE
(
	sigsub_SSEFfectAura,
	0x14ABFE6A0,
	"\x40\x53\x48\x83\xEC\x20\x48\x89\xCB\xE8\xCC\xCC\xCC\xCC\x48\x8D\x05\xCC\xCC\xCC\xCC\x48\x8D\x4B\x48", 
	"xxxxxxxxxx????xxx????xxxx"
)


CL_SCAN_SIGNATURE
(
	sigsub_SSAuraDestructor,
	0x140896750,
	"\x48\x83\xEC\x28\x48\x89\x7C\x24\x20\x0F",
	"xxxxxxxxxx"
)


CL_SCAN_SIGNATURE
(
	sigPlayBGM,
	0x1409F7C00,
	"\x4C\x8B\xC2\x33\xD2\xE9\x06", 
	"xxxxxxx"
);

//updated
CL_SCAN_SIGNATURE
(
	sigSetNewMsg,
	0x1525038D0,
	"\x48\x8D\x05\xCC\xCC\xCC\xCC\x89\x51\x08\x48\x89\x01\x31", 
	"xxx????xxxxxxx"
)

/**CL_SCAN_SIGNATURE
(
	sigGetCurIsland_,
	0x140260500,
	"\xE8\xCC\xCC\xCC\xCC\x83\xF8\x05\x75\x34", 
	"x????xxxxx"
)*/

CL_SCAN_SIGNATURE
(
	sig_TitanSSManage,
	0x14099C270,
	"\x48\x89\x5C\x24\x18\x48\x89\x6C\x24\x20\x57\x48\x83\xEC\x50\x0F", 
	"xxxxxxxxxxxxxxxx"
)

/*/SIG_SCAN
(
	sig_SetBoostValue,
	0x1408D45EA,
	"\xF3\x0F\x11\x4F\x3C\x48\x8B\xCF", 
	"xxxxxxxx"
)*/