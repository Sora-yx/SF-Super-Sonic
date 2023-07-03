#pragma once

extern int curState;

#define STATUS_PARAM_BOOST          0x01
#define STATUS_PARAM_RECOVERYJUMP   0x02
#define STATUS_PARAM_AIRBOOST       0x04
#define STATUS_PARAM_GRINDJUMP      0x06
#define STATUS_PARAM_GRIND          0x07
#define STATUS_PARAM_JUMP           0x08
#define STATUS_PARAM_DOUBLEJUMP     0x09
#define STATUS_PARAM_STOMPBOUNCE    0x0A
#define STATUS_PARAM_FALL		    0x0B
#define STATUS_PARAM_STOMP          0x0C
#define STATUS_PARAM_SKYDIVE        0x0D
#define STATUS_PARAM_SKYDIVEBOOST   0x0E
#define STATUS_PARAM_CYLOOP         0x11
#define STATUS_PARAM_CYLOOPEND      0x12
#define STATUS_PARAM_DRIFT          0x13
#define STATUS_PARAM_DRIFTDASH      0x14
#define STATUS_PARAM_HOMING         0x17
#define STATUS_PARAM_PARRY          0x18
#define STATUS_PARAM_WALLCLIMB      0x19
#define STATUS_PARAM_IDLE		    0x1A
#define STATUS_PARAM_BOARDING       0x1E
#define STATUS_PARAM_IMPULSE_TARGET 0x21
#define STATUS_PARAM_PHANTOMRUSH    0x26

#define STATUS_30_SIDEVIEW    0x1A
#define STATUS_30_POWERBOOST  0x27
#define STATUS_30_ISLANDSTAGE 0x31

class BlackboardHelper
{
public:
	enum EStateFlags : int64_t
	{
		EStateFlags_IsBoost = 0x01,
		EStateFlags_IsRecoveryJump = 0x02,
		EStateFlags_IsAirBoost = 0x04,
		EStateFlags_IsGrindJump = 0x06,
		EStateFlags_IsGrind = 0x07,
		EStateFlags_IsJump = 0x08,
		EStateFlags_IsDoubleJump = 0x09,
		EStateFlags_IsBounceJump = 0x0A,
		EStateFlags_IsFall = 0x0B,
		EStateFlags_IsStomp = 0x0C,
		EStateFlags_IsDiving = 0x0D,
		EStateFlags_IsDivingBoost = 0x0E,
		EStateFlags_IsCyloop = 0x11,
		EStateFlags_IsCyloopEnd = 0x12,
		EStateFlags_IsDrift = 0x13,
		EStateFlags_IsDriftDash = 0x14,
		EStateFlags_IsHoming = 0x17,
		EStateFlags_IsParry = 0x18,
		EStateFlags_IsWallClimb = 0x19,
		EStateFlags_IsIdle = 0x1A,
		EStateFlags_IsBoarding = 0x1E,
		EStateFlags_IsSpringJump = 0x21,
		EStateFlags_IsPhantomRush = 0x26
	};

	enum EWorldFlags : int64_t
	{
		EWorldFlags_IsCyberSpace = 0x1D,
		EWorldFlags_IsPowerBoost = 0x26,
		EWorldFlags_IsHeightMapCollision = 0x31
	};

	inline static app::player::BlackboardStatus* GetStatus()
	{
		auto* pGameDocument = app::GameDocument::GetSingleton();

		if (!pGameDocument)
			return nullptr;

		auto* pSonic = pGameDocument->GetGameObject<app::player::Sonic>();

		if (!pSonic)
			return nullptr;

		auto* pPlayerBlackboard = pSonic->GetComponent<app::player::GOCPlayerBlackboard>();

		if (!pPlayerBlackboard)
			return nullptr;

		auto* pBlackboardStatus = pPlayerBlackboard->pBlackboard->GetBlackboardContent<app::player::BlackboardStatus>();

		if (!pBlackboardStatus)
			return nullptr;

		return pBlackboardStatus;
	}

	inline static bool CheckStateFlags(EStateFlags in_flags)
	{
		auto status = GetStatus();

		if (!status)
			return false;

		return _bittest64(&status->StateFlags, in_flags);
	}

	inline static bool CheckWorldFlags(EWorldFlags in_flags)
	{
		auto status = GetStatus();

		if (!status)
			return false;

		return _bittest64(&status->WorldFlags, in_flags);
	}

	inline static bool IsAirBoosting()
	{
		return CheckStateFlags(EStateFlags::EStateFlags_IsAirBoost);
	}

	inline static bool IsBoosting()
	{
		return CheckStateFlags(EStateFlags::EStateFlags_IsBoost);
	}

	inline static bool IsCyberSpace()
	{
		return CheckWorldFlags(EWorldFlags::EWorldFlags_IsCyberSpace);
	}

	inline static bool IsCylooping()
	{
		return CheckStateFlags(EStateFlags::EStateFlags_IsCyloop);
	}

	inline static bool IsDrifting()
	{
		return CheckStateFlags(EStateFlags::EStateFlags_IsDrift);
	}

	inline static bool IsGrinding()
	{
		return CheckStateFlags(EStateFlags::EStateFlags_IsGrind);
	}

	inline static bool IsJumping()
	{
		return CheckStateFlags(EStateFlags::EStateFlags_IsJump);
	}

	inline static bool IsWallClimbing()
	{
		return CheckStateFlags(EStateFlags::EStateFlags_IsWallClimb);
	}

	inline static bool IsSuper()
	{
		return GetStatus()->isSuper;
	}

	inline static bool IsFlyingAsSS()
	{
		return isSuper && curState == 102;
	}
};