#pragma once

enum state
{
	StandState,
	FlyState = 100,
};

enum songs
{
	sonic2,
	sonic3,
	sk,
	mania,
	sa1,
	sa2,
	heroes,
	ft0,
	ft1,
	ft2,
	maintheme,
	titans,
	random,
};

extern bool nolimit;
extern int AscendBtn;
extern int DescendBtn;
extern int TransformBtn;
extern int UntransformBtn;
extern bool hedgeMayCry;

extern std::string AscendKey;
extern std::string DescendKey;
extern std::string TransformKey;
extern std::string UntransformKey;

void Init_Config();
extern bool isSuper;
extern uint8_t useSSMusic;

extern std::string modPath;

extern int currentSuperFormIndex;
extern float vol;
