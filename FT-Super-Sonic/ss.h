#pragma once


void ResetValues();
void ForceUnTransfo(bool resetContext);

class SuperSonic
{
public:
	static void Init();
	static void Transfo_CheckInput(SonicContext* Scontext);
	static void ringLoss(SonicContext* Scontext);
	static void Ascend_CheckInput(SonicContext* Scontext, GOCKinematicPrams* param);
	static void Descend_CheckInput(GOCKinematicPrams* param);
	static void Transfo(SonicContext* SContext);
	static void Untransfo(SonicContext* SContext);
	static void OnFrames();
};