#pragma once


extern SonicContext* sonicContextPtr;

void init_SuperSonicHacks();
void SuperSonic_OnFrames(SonicContext* SContext);

class SuperSonic
{
public:
	static void Init();
	static void Transfo_CheckInput(SonicContext* Scontext);
	static void ringLoss(SonicContext* Scontext);
	static void Ascend_CheckInput(SonicContext* Scontext, GOCKinematicPrams* param);
	static void Descend_CheckInput(GOCKinematicPrams* param);
	static void Ground_Check(SonicContext* SContext);
	static void Transfo(SonicContext* SContext);
	static void Untransfo(SonicContext* SContext);
	static void OnFrames(SonicContext* SContext);
};