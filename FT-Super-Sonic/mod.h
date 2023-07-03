#pragma once

enum state
{
	StandState,
	FlyState = 100,
};

extern bool nolimit;
extern int AscendBtn;
extern int DescendBtn;
extern int TransformBtn;
extern int UntransformBtn;

extern std::string  AscendKey;
extern std::string  DescendKey;
extern std::string  TransformKey;
extern std::string  UntransformKey;

void Init_Config();
extern bool isSuper;
extern uint8_t useSSMusic;

extern std::string modPath;
