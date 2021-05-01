#pragma once
// used: winapi includes, singleton
#include "../common.h"
// used: usercmd
#include "../sdk/datatypes/usercmd.h"
// used: angle
#include "../sdk/datatypes/qangle.h"
// used: baseentity
#include "../sdk/entity.h"

class CAntiAim : public CSingleton<CAntiAim>
{
public:
	// Get
	void Run(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket);
	/* align local animation state to server */
	void UpdateServerAnimations(CUserCmd* pCmd, CBaseEntity* pLocal); // @credits: pazzo

	// Values
	/* angles modified by antiaim and being sent */
	QAngle angSentView = { };
private:
	// Main
	void Pitch(CUserCmd* pCmd, CBaseEntity* pLocal);
	void Yaw(CUserCmd* pCmd, CBaseEntity* pLocal, float flServerTime, bool& bSendPacket);
	void CorrectMovement(float OldAngleY, CUserCmd* pCmd, float fOldForward, float fOldSidemove);

	// Extra
	/* returns max server desynchronization angle delta */
	float GetMaxDesyncDelta(CBasePlayerAnimState* pAnimState); // @credits: sharklaser1's reversed setupvelocity

	void Desync(CUserCmd* pCmd, float flServerTime, float flMaxDesyncDelta, bool& bSendPacket);
	void Jitter(CUserCmd* pCmd, float flServerTime, float flMaxDesyncDelta, bool& bSendPacket);
	void BackJitter(CUserCmd* pCmd);

	// Values
	/* updated by server animstate */
	CBasePlayerAnimState* pServerAnimState = nullptr;
	/* next lower body yaw update time from server */
	float flNextLowerBodyUpdate = 0.f;
	float lbyNextUpdate = 0.f;
	QAngle LastAngle;
};
