#include "triggerbot.h"

// used: cheat variables
#include "../core/variables.h"
// used: max distance, classindex, itemdefinitionindex
#include "../sdk/definitions.h"
// used: angle/vector calculations
#include "../utilities/math.h"
// used: is key pressed for bind
#include "../utilities/inputsystem.h"
// used: convar class, globals interface
#include "../core/interfaces.h"
// used: firebullet data
#include "autowall.h"

void CTriggerBot::UpdateSettings(CBaseEntity* pLocal) {
	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();
	short nDefinitionIndex = pWeapon->GetItemDefinitionIndex();
	CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nDefinitionIndex);
	iTriggerKey = C::Get<int>(Vars.iTriggerKey);
	bTriggerAlwaysActive = C::Get<bool>(Vars.bTriggerAlwaysActive);

	if (pWeaponData->nWeaponType == WEAPONTYPE_PISTOL && C::Get<bool>(Vars.bTriggerPistol)) {
		iTriggerDelay = C::Get<int>(Vars.iTriggerDelayPistol);
		bTriggerAutoWall = C::Get<bool>(Vars.bTriggerAutoWallPistol);
		iTriggerMinimalDamage = C::Get<int>(Vars.iTriggerMinimalDamagePistol);
	}
	else if (pWeaponData->nWeaponType == WEAPONTYPE_SNIPER && C::Get<bool>(Vars.bTriggerSniper)) {
		iTriggerDelay = C::Get<int>(Vars.iTriggerDelaySniper);
		bTriggerAutoWall = C::Get<bool>(Vars.bTriggerAutoWallSniper);
		iTriggerMinimalDamage = C::Get<int>(Vars.iTriggerMinimalDamageSniper);
	}
	else if (pWeaponData->nWeaponType == WEAPONTYPE_SUBMACHINEGUN && C::Get<bool>(Vars.bTriggerSMG)) {
		iTriggerDelay = C::Get<int>(Vars.iTriggerDelaySMG);
		bTriggerAutoWall = C::Get<bool>(Vars.bTriggerAutoWallSMG);
		iTriggerMinimalDamage = C::Get<int>(Vars.iTriggerMinimalDamageSMG);
	}
	else if (pWeaponData->nWeaponType == WEAPONTYPE_RIFLE && C::Get<bool>(Vars.bTriggerRifle)) {
		iTriggerDelay = C::Get<int>(Vars.iTriggerDelayRifle);
		bTriggerAutoWall = C::Get<bool>(Vars.bTriggerAutoWallRifle);
		iTriggerMinimalDamage = C::Get<int>(Vars.iTriggerMinimalDamageRifle);
	}
	else if ((pWeaponData->nWeaponType == WEAPONTYPE_MACHINEGUN || pWeaponData->nWeaponType == WEAPONTYPE_SHOTGUN) && C::Get<bool>(Vars.bTriggerHeavy)) {
		iTriggerDelay = C::Get<int>(Vars.iTriggerDelayHeavy);
		bTriggerAutoWall = C::Get<bool>(Vars.bTriggerAutoWallHeavy);
		iTriggerMinimalDamage = C::Get<int>(Vars.iTriggerMinimalDamageHeavy);
	}
	else {
		iTriggerDelay = C::Get<int>(Vars.iTriggerDelay);
		bTriggerAutoWall = C::Get<bool>(Vars.bTriggerAutoWall);
		iTriggerMinimalDamage = C::Get<int>(Vars.iTriggerMinimalDamage);
	}
}

void CTriggerBot::Run(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	if (!pLocal->IsAlive())
		return;

	CTriggerBot::UpdateSettings(pLocal);
	
	if ((iTriggerKey > 0 && IPT::IsKeyDown(iTriggerKey)) || bTriggerAlwaysActive)
	{
		CTriggerBot::TriggerBot(pCmd, pLocal);
	}
	else {
		timer.Reset();
		return;
	}
}

void CTriggerBot::TriggerBot(CUserCmd* pCmd, CBaseEntity* pLocal) {
	static CConVar* weapon_recoil_scale = I::ConVar->FindVar(XorStr("weapon_recoil_scale"));

	if (weapon_recoil_scale == nullptr)
		return;

	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	if (pWeapon == nullptr)
		return;

	short nDefinitionIndex = pWeapon->GetItemDefinitionIndex();
	CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nDefinitionIndex);

	// check is weapon gun
	if (pWeaponData == nullptr || !pWeaponData->IsGun())
		return;

	// get view and add punch
	QAngle angView = pCmd->angViewPoint;
	angView += pLocal->GetPunch() * weapon_recoil_scale->GetFloat();

	Vector vecStart, vecEnd, vecForward;
	M::AngleVectors(angView, &vecForward);

	vecStart = pLocal->GetEyePosition();
	vecForward *= pWeaponData->flRange;
	vecEnd = vecStart + vecForward;

	Trace_t trace = { };
	if (bTriggerAutoWall)
	{
		FireBulletData_t data = { };

		// get autowall damage and data from it
		float flDamage = CAutoWall::Get().GetDamage(pLocal, vecEnd, data);

		// check for minimal damage
		if (flDamage < iTriggerMinimalDamage)
			return;

		// copy trace from autowall
		trace = data.enterTrace;
	}
	else
	{
		// otherwise ray new trace
		Ray_t ray(vecStart, vecEnd);
		CTraceFilter filter(pLocal);
		I::EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);
	}

	CBaseEntity* pEntity = trace.pHitEntity;

	// check is trace player valid and enemy
	if (pEntity == nullptr || !pEntity->IsAlive() || pEntity->IsDormant() || !pEntity->IsPlayer() || pEntity->HasImmunity() || !pLocal->IsEnemy(pEntity))
	{
		timer.Reset();
		return;
	}

	// hitgroup filters check
		// head
	if ((C::Get<bool>(Vars.bTriggerHead) && trace.iHitGroup == HITGROUP_HEAD) ||
		// chest
		(C::Get<bool>(Vars.bTriggerChest) && trace.iHitGroup == HITGROUP_CHEST) ||
		// stomach
		(C::Get<bool>(Vars.bTriggerStomach) && trace.iHitGroup == HITGROUP_STOMACH) ||
		// arms
		(C::Get<bool>(Vars.bTriggerArms) && (trace.iHitGroup == HITGROUP_LEFTARM || trace.iHitGroup == HITGROUP_RIGHTARM)) ||
		// legs
		(C::Get<bool>(Vars.bTriggerLegs) && (trace.iHitGroup == HITGROUP_LEFTLEG || trace.iHitGroup == HITGROUP_RIGHTLEG)))
	{
		if (pLocal->CanShoot(static_cast<CWeaponCSBase*>(pWeapon)))
		{
			// check is delay elapsed
			if (iTriggerDelay > 0)
			{
				if (timer.Elapsed() < iTriggerDelay)
					return;
			}

			pCmd->iButtons |= IN_ATTACK;
		}
	}
}