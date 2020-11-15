#pragma once
// used: winapi includes, singleton
#include "../common.h"
// used: usercmd
#include "../sdk/datatypes/usercmd.h"
// used: angle
#include "../sdk/datatypes/qangle.h"
// used: baseentity
#include "../sdk/entity.h"

// @note: FYI - https://www.unknowncheats.me/forum/general-programming-and-reversing/173986-math-hack-2-predicting-future-3d-kinematics.html

class CRageBot : public CSingleton<CRageBot>
{
public:
	// Get
	void Run(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket);
	void AutoRevolver(CUserCmd* pCmd, CBaseEntity* pLocal);

	// Global Values
	CBaseEntity* pBestEntity = nullptr;
private:
	void UpdateSettings(CUserCmd* pCmd, CBaseEntity* pLocal);
	void Rage(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket);
	void DoAimbot(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket);
	int HitChance(QAngle angles, CBaseEntity* entity, CBaseCombatWeapon* pWeapon, CBaseEntity* pLocal);
	bool IsAbleToShoot(CBaseEntity* pLocal);
	bool CanOpenFire(CBaseEntity* pLocal);
	Vector BestPoint(CBaseEntity* targetPlayer, Vector & final, CBaseEntity* pLocal);
	bool TargetMeetsRequirements(CBaseEntity* pEntity, CBaseEntity* pLocal);
	float FovToPlayer(Vector ViewOffSet, QAngle View, CBaseEntity* pEntity, int aHitBox);
	int GetTargetCrosshair(CBaseEntity* pLocal);
	int GetTargetDistance(CBaseEntity* pLocal);
	int GetTargetHealth(CBaseEntity* pLocal);
	int HitScan(CBaseEntity* pEntity, CBaseEntity* pLocal);
	void DoNoRecoil(CBaseEntity* pLocal);
	void PositionAdjustment(CUserCmd* pCmd, CBaseEntity* pLocal);
	bool AimAtPoint(CBaseEntity* pLocal, Vector point, CUserCmd* pCmd, bool& bSendPacket);
	void Autostop(CUserCmd* cmd, CBaseEntity* pLocal);

	int ihitchance;
	int imin_damage;
	int itarget;
	bool bAimStep;
	bool bPreferBodyAim;
	int iAimType;
	bool bPositionAdjustment;
	bool bAimbotEnable;
	bool bDoNoRecoil;
	bool bAutoScope;
	bool bAimbotAutoFire;
	bool bAccuracyAutoStop;
	bool bAimbotAutoPistol;
	bool bBestPoint;
	int iTargetSelection;
	int iSmart;
	int iAimScanType;
	CBaseEntity* Target;
	QAngle LastAngle;
	int Shots;
	bool bHitboxHead;
	bool bHitboxChest;
	bool bHitboxStomach;
	bool bAutoWall;
	bool bHitboxArms;
	bool bHitboxlegs;
	float flPointScale;
	float flHeadPointScale;
	float flChestPointScale;

	bool IsAimStepping;
	QAngle LastAimstepAngle;

	bool IsLocked; 
	int TargetID;
	int HitBox;
	Vector AimPoint;
	CUserCmd* cmd;

	static float g_CapsuleVertices[][3];
};
