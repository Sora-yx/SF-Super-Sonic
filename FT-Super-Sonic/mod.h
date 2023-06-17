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

extern char AscendKey;
extern char DescendKey;
extern char TransformKey;
extern char UntransformKey;

void Init_Config();
extern bool isSuper;
extern uint8_t useSSMusic;
