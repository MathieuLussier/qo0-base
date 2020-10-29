#pragma once
// used: winapi includes, singleton
#include "../common.h"
// used: usercmd
#include "../sdk/datatypes/usercmd.h"
// used: baseentity, baseweapon
#include "../sdk/entity.h"
// used: timer
#include "../utilities.h"

class CTriggerBot : public CSingleton<CTriggerBot>
{
public:
	// Get
	void Run(CUserCmd* pCmd, CBaseEntity* pLocal);
private:
	void TriggerBot(CUserCmd* pCmd, CBaseEntity* pLocal);
	void UpdateSettings(CBaseEntity* pLocal);
	// Values
	/* timer for delay before shoot */
	bool bTriggerAlwaysActive;
	int iTriggerKey;
	int iTriggerDelay;
	bool bTriggerAutoWall;
	int iTriggerMinimalDamage;
	CTimer timer = { };
};
