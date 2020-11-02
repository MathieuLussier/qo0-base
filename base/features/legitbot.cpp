#include "legitbot.h"

#include "../global.h"
// used: cheat variables
#include "../core/variables.h"
// used: main window open state
#include "../core/menu.h"
// used: key state
#include "../utilities/inputsystem.h"
// used: globals, cliententitylist interfaces
#include "../core/interfaces.h"
// used: backtracking
#include "lagcompensation.h"

#include "../utilities.h"

#include "../utilities/math.h"

#include "../utilities/logging.h"

void CLegitBot::UpdateSettings(CBaseEntity* pLocal) {
	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();
	short nDefinitionIndex = pWeapon->GetItemDefinitionIndex();
	CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nDefinitionIndex);
	iAimbotKey = C::Get<int>(Vars.iAimbotKey);

	if (pWeaponData->nWeaponType == WEAPONTYPE_PISTOL && C::Get<bool>(Vars.bAimbotPistol)) {
		bAimbotBetweenShots = C::Get<bool>(Vars.bAimbotBetweenShotsPistol);
		flLegitFov = C::Get<float>(Vars.flLegitFovPistol);
		flLegitSmooth = C::Get<float>(Vars.flLegitSmoothPistol);

		std::vector<bool> hitboxes = C::Get<std::vector<bool>>(Vars.vecPistolHitboxes);
		bHitboxHead = hitboxes[0];
		bHitboxChest = hitboxes[1];
		bHitboxStomach = hitboxes[2];
		bHitboxArms = hitboxes[3];
		bHitboxlegs = hitboxes[4];

		bLegitSilent = C::Get<bool>(Vars.bLegitSilentPistol);
		flLegitSilentFov = C::Get<float>(Vars.flLegitSilentFovPistol);

		flLegitRecoilX = C::Get<float>(Vars.flLegitRecoilXPistol);
		flLegitRecoilY = C::Get<float>(Vars.flLegitRecoilYPistol);
	}
	else if (pWeaponData->nWeaponType == WEAPONTYPE_SNIPER && C::Get<bool>(Vars.bAimbotSniper)) {
		bAimbotBetweenShots = C::Get<bool>(Vars.bAimbotBetweenShotsSniper);
		flLegitFov = C::Get<float>(Vars.flLegitFovSniper);
		flLegitSmooth = C::Get<float>(Vars.flLegitSmoothSniper);

		std::vector<bool> hitboxes = C::Get<std::vector<bool>>(Vars.vecSniperHitboxes);
		bHitboxHead = hitboxes[0];
		bHitboxChest = hitboxes[1];
		bHitboxStomach = hitboxes[2];
		bHitboxArms = hitboxes[3];
		bHitboxlegs = hitboxes[4];

		bLegitSilent = C::Get<bool>(Vars.bLegitSilentSniper);
		flLegitSilentFov = C::Get<float>(Vars.flLegitSilentFovSniper);

		flLegitRecoilX = C::Get<float>(Vars.flLegitRecoilXSniper);
		flLegitRecoilY = C::Get<float>(Vars.flLegitRecoilYSniper);
	}
	else if (pWeaponData->nWeaponType == WEAPONTYPE_SUBMACHINEGUN && C::Get<bool>(Vars.bAimbotSMG)) {
		bAimbotBetweenShots = C::Get<bool>(Vars.bAimbotBetweenShotsSMG);
		flLegitFov = C::Get<float>(Vars.flLegitFovSMG);
		flLegitSmooth = C::Get<float>(Vars.flLegitSmoothSMG);

		std::vector<bool> hitboxes = C::Get<std::vector<bool>>(Vars.vecSMGHitboxes);
		bHitboxHead = hitboxes[0];
		bHitboxChest = hitboxes[1];
		bHitboxStomach = hitboxes[2];
		bHitboxArms = hitboxes[3];
		bHitboxlegs = hitboxes[4];

		bLegitSilent = C::Get<bool>(Vars.bLegitSilentSMG);
		flLegitSilentFov = C::Get<float>(Vars.flLegitSilentFovSMG);

		flLegitRecoilX = C::Get<float>(Vars.flLegitRecoilXSMG);
		flLegitRecoilY = C::Get<float>(Vars.flLegitRecoilYSMG);
	}
	else if (pWeaponData->nWeaponType == WEAPONTYPE_RIFLE && C::Get<bool>(Vars.bAimbotRifle)) {
		bAimbotBetweenShots = C::Get<bool>(Vars.bAimbotBetweenShotsRifle);
		flLegitFov = C::Get<float>(Vars.flLegitFovRifle);
		flLegitSmooth = C::Get<float>(Vars.flLegitSmoothRifle);

		std::vector<bool> hitboxes = C::Get<std::vector<bool>>(Vars.vecRifleHitboxes);
		bHitboxHead = hitboxes[0];
		bHitboxChest = hitboxes[1];
		bHitboxStomach = hitboxes[2];
		bHitboxArms = hitboxes[3];
		bHitboxlegs = hitboxes[4];

		bLegitSilent = C::Get<bool>(Vars.bLegitSilentRifle);
		flLegitSilentFov = C::Get<float>(Vars.flLegitSilentFovRifle);

		flLegitRecoilX = C::Get<float>(Vars.flLegitRecoilXRifle);
		flLegitRecoilY = C::Get<float>(Vars.flLegitRecoilYRifle);
	}
	else if ((pWeaponData->nWeaponType == WEAPONTYPE_MACHINEGUN || pWeaponData->nWeaponType == WEAPONTYPE_SHOTGUN) && C::Get<bool>(Vars.bAimbotHeavy)) {
		bAimbotBetweenShots = C::Get<bool>(Vars.bAimbotBetweenShotsHeavy);
		flLegitFov = C::Get<float>(Vars.flLegitFovHeavy);
		flLegitSmooth = C::Get<float>(Vars.flLegitSmoothHeavy);

		std::vector<bool> hitboxes = C::Get<std::vector<bool>>(Vars.vecHeavyHitboxes);
		bHitboxHead = hitboxes[0];
		bHitboxChest = hitboxes[1];
		bHitboxStomach = hitboxes[2];
		bHitboxArms = hitboxes[3];
		bHitboxlegs = hitboxes[4];

		bLegitSilent = C::Get<bool>(Vars.bLegitSilentHeavy);
		flLegitSilentFov = C::Get<float>(Vars.flLegitSilentFovHeavy);

		flLegitRecoilX = C::Get<float>(Vars.flLegitRecoilXHeavy);
		flLegitRecoilY = C::Get<float>(Vars.flLegitRecoilYHeavy);
	}
	else {
		iAimbotKey = C::Get<int>(Vars.iAimbotKey);
		bAimbotBetweenShots = C::Get<bool>(Vars.bAimbotBetweenShots);
		flLegitFov = C::Get<float>(Vars.flLegitFov);
		flLegitSmooth = C::Get<float>(Vars.flLegitSmooth);

		std::vector<bool> hitboxes = C::Get<std::vector<bool>>(Vars.vecAimbotHitboxes);
		bHitboxHead = hitboxes[0];
		bHitboxChest = hitboxes[1];
		bHitboxStomach = hitboxes[2];
		bHitboxArms = hitboxes[3];
		bHitboxlegs = hitboxes[4];

		bLegitSilent = C::Get<bool>(Vars.bLegitSilent);
		flLegitSilentFov = C::Get<float>(Vars.flLegitSilentFov);

		flLegitRecoilX = C::Get<float>(Vars.flLegitRecoilX);
		flLegitRecoilY = C::Get<float>(Vars.flLegitRecoilY);
	}
}

void CLegitBot::Run(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket)
{
	if (!pLocal->IsAlive())
		return;

	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	if (pWeapon == nullptr)
		return;

	short nDefinitionIndex = pWeapon->GetItemDefinitionIndex();
	CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nDefinitionIndex);

	if (pWeaponData == nullptr)
		return;

	static CConVar* weapon_recoil_scale = I::ConVar->FindVar(XorStr("weapon_recoil_scale"));

	if (weapon_recoil_scale == nullptr)
		return;

	if (!pLocal->CanShoot(static_cast<CWeaponCSBase*>(pWeapon)) || pWeapon->GetAmmo() == 0 || !pWeapon->IsWeapon() || pWeaponData->nWeaponType == WEAPONTYPE_KNIFE || pWeaponData->nWeaponType == WEAPONTYPE_GRENADE || pWeaponData->nWeaponType == WEAPONTYPE_FISTS || pWeaponData->nWeaponType == WEAPONTYPE_C4)
		return;

	// disable when in-menu for more legit (lol)
	if (W::bMainOpened)
		return;

	CLegitBot::UpdateSettings(pLocal);

	if (((iAimbotKey > 0 && IPT::IsKeyDown(iAimbotKey)) || bAimbotBetweenShots) && pLocal->GetShotsFired() <= 1)
		CLegitBot::Aimbot(pCmd, pLocal, bSendPacket);

	if (pLocal->GetShotsFired() > 1) {
		CLegitBot::RCS(pCmd, pLocal);
	}
	else {
		v_old_punch = { 0.0f, 0.0f, 0.0f };
	}
}

void CLegitBot::Aimbot(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket) {

	int best_ent_index = CLegitBot::GetClosestByCrosshair(pLocal);

	if (best_ent_index > -1) {
		CBaseEntity* ent_toAim = I::ClientEntityList->Get<CBaseEntity>(best_ent_index);

		EHitboxIndex closest_hitbox = CLegitBot::findClosestHitBox(pLocal, ent_toAim);

		if (closest_hitbox > HITBOX_INVALID) {
			CBasePlayerAnimState* local_anim_state = pLocal->GetAnimationState();
			Vector local_head_pos = pLocal->GetEyePosition();
			local_head_pos.x += (local_anim_state->vecVelocity.x * I::Globals->flIntervalPerTick);
			local_head_pos.y += (local_anim_state->vecVelocity.y * I::Globals->flIntervalPerTick);
			CBasePlayerAnimState* anim_state = ent_toAim->GetAnimationState();
			Vector aim_pos = ent_toAim->GetHitboxPosition(closest_hitbox).value();
			aim_pos.x += (anim_state->vecVelocity.x * I::Globals->flIntervalPerTick);
			aim_pos.y += (anim_state->vecVelocity.y * I::Globals->flIntervalPerTick);
			QAngle angles_to_aim = M::CalcAngle(local_head_pos, aim_pos).Normalize();
			
			if (!pLocal->IsVisible(ent_toAim, aim_pos, true) || U::LineGoesThroughSmoke(local_head_pos, aim_pos))
				return;

			if (bLegitSilent && CLegitBot::EnnemieInFov(pLocal->GetAbsAngles(), angles_to_aim, flLegitSilentFov)) {
				pCmd->angViewPoint = angles_to_aim;
			}
			else {
				pCmd->angViewPoint = CLegitBot::Smooth(pLocal->GetAbsAngles(), angles_to_aim);
				I::Engine->SetViewAngles(pCmd->angViewPoint);
			}
		}
	}
}

void CLegitBot::RCS(CUserCmd* pCmd, CBaseEntity* pLocal) {
	static CConVar* weapon_recoil_scale = I::ConVar->FindVar(XorStr("weapon_recoil_scale"));

	auto v_aim_punch = pLocal->GetPunch() * weapon_recoil_scale->GetFloat();

	v_aim_punch.x *= flLegitRecoilX;
	v_aim_punch.y *= flLegitRecoilY;

	auto v_rcs = pCmd->angViewPoint += (v_old_punch - v_aim_punch);

	I::Engine->SetViewAngles(v_rcs.Normalize());
	v_old_punch = v_aim_punch;
}

bool CLegitBot::CheckIfWall(CUserCmd* pCmd, CBaseEntity* pLocal, QAngle angles_to_aim)
{
	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	short nDefinitionIndex = pWeapon->GetItemDefinitionIndex();
	CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nDefinitionIndex);

	static CConVar* weapon_recoil_scale = I::ConVar->FindVar(XorStr("weapon_recoil_scale"));

	pCmd->angViewPoint = angles_to_aim;

	QAngle angView = pCmd->angViewPoint;
	angView += pLocal->GetPunch() * weapon_recoil_scale->GetFloat();

	Vector vecStart, vecEnd, vecForward;
	M::AngleVectors(angView, &vecForward);

	vecStart = pLocal->GetEyePosition();
	vecForward *= pWeaponData->flRange;
	vecEnd = vecStart + vecForward;

	Trace_t trace = { };
	Ray_t ray(vecStart, vecEnd);
	CTraceFilter filter(pLocal);
	I::EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

	CBaseEntity* pEntity = trace.pHitEntity;

	if (pEntity == nullptr || !pEntity->IsAlive() || pEntity->IsDormant() || !pEntity->IsPlayer() || pEntity->HasImmunity() || !pLocal->IsEnemy(pEntity))
	{
		return true;
	}

	return false;
}

QAngle CLegitBot::Smooth(QAngle angles, QAngle target_angles)
{
	float smooth = flLegitSmooth;
	if (smooth < 1.f)
		return target_angles;
	QAngle delta;
	delta.x = target_angles.x - angles.x;
	delta.y = target_angles.y - angles.y;
	delta.z = 0.f;
	delta.Normalize();
	QAngle new_angle;
	new_angle.x = angles.x + delta.x / smooth;
	new_angle.y = angles.y + delta.y / smooth;
	new_angle.z = 0.f;
	return new_angle.Normalize();
}

int CLegitBot::GetClosestByCrosshair(CBaseEntity* pLocal)
{
	float fClosets = std::numeric_limits<float>::max();
	int iIndex = -1;
	for (int i = 1; i <= I::Globals->nMaxClients; i++)
	{
		CBaseEntity* pEntity = I::ClientEntityList->Get<CBaseEntity>(i);

		if (pEntity == nullptr || !pEntity->IsPlayer() || pEntity->IsDormant() || !pEntity->IsAlive() || pEntity->HasImmunity())
			continue;

		if (pEntity->GetTeam() == pLocal->GetTeam())
			continue;

		Vector local_head_pos = pLocal->GetEyePosition();

		EHitboxIndex closest_hitbox = CLegitBot::findClosestHitBox(pLocal, pEntity);

		if (closest_hitbox > HITBOX_INVALID) {
			Vector aim_pos = pEntity->GetHitboxPosition(closest_hitbox).value();
			QAngle angles_to_aim = M::CalcAngle(local_head_pos, aim_pos).Normalize();
			float fDistance = CLegitBot::CalcAnglesDistance(pLocal->GetAbsAngles(), angles_to_aim);

			if (fDistance < fClosets)
			{
				fClosets = fDistance;
				iIndex = i;
			}
		}
	}
	return iIndex;
}

EHitboxIndex CLegitBot::findClosestHitBox(CBaseEntity* pLocal, CBaseEntity* pEntity) {
	float fClosets = std::numeric_limits<float>::max();
	EHitboxIndex iIndex = HITBOX_INVALID;

	Vector local_head_pos = pLocal->GetEyePosition();

	if (bHitboxHead) {
		Vector head_pos = pEntity->GetHitboxPosition(HITBOX_HEAD).value();

		QAngle angles_to_aim = M::CalcAngle(local_head_pos, head_pos).Normalize();
		if (EnnemieInFov(pLocal->GetAbsAngles(), angles_to_aim, flLegitFov))
		{
			float fDistance = CLegitBot::CalcAnglesDistance(pLocal->GetAbsAngles(), angles_to_aim);

			if (fDistance < fClosets)
			{
				fClosets = fDistance;
				iIndex = HITBOX_HEAD;
			}
		}
	}

	if (bHitboxChest) {
		Vector chest_pos = pEntity->GetHitboxPosition(HITBOX_CHEST).value();

		QAngle angles_to_aim = M::CalcAngle(local_head_pos, chest_pos).Normalize();
		if (EnnemieInFov(pLocal->GetAbsAngles(), angles_to_aim, flLegitFov))
		{
			float fDistance = CLegitBot::CalcAnglesDistance(pLocal->GetAbsAngles(), angles_to_aim);

			if (fDistance < fClosets)
			{
				fClosets = fDistance;
				iIndex = HITBOX_CHEST;
			}
		}
	}

	if (bHitboxStomach) {
		Vector stomach_pos = pEntity->GetHitboxPosition(HITBOX_STOMACH).value();

		QAngle angles_to_aim = M::CalcAngle(local_head_pos, stomach_pos).Normalize();
		if (EnnemieInFov(pLocal->GetAbsAngles(), angles_to_aim, flLegitFov))
		{
			float fDistance = CLegitBot::CalcAnglesDistance(pLocal->GetAbsAngles(), angles_to_aim);

			if (fDistance < fClosets)
			{
				fClosets = fDistance;
				iIndex = HITBOX_STOMACH;
			}
		}
	}

	if (bHitboxArms) {
		Vector left_arm_pos = pEntity->GetHitboxPosition(HITBOX_LEFT_UPPER_ARM).value();

		QAngle angles_to_aim = M::CalcAngle(local_head_pos, left_arm_pos).Normalize();
		if (EnnemieInFov(pLocal->GetAbsAngles(), angles_to_aim, flLegitFov))
		{
			float fDistance = CLegitBot::CalcAnglesDistance(pLocal->GetAbsAngles(), angles_to_aim);

			if (fDistance < fClosets)
			{
				fClosets = fDistance;
				iIndex = HITBOX_LEFT_UPPER_ARM;
			}
		}

		Vector right_arm_pos = pEntity->GetHitboxPosition(HITBOX_RIGHT_UPPER_ARM).value();
		angles_to_aim = M::CalcAngle(local_head_pos, right_arm_pos).Normalize();
		if (EnnemieInFov(pLocal->GetAbsAngles(), angles_to_aim, flLegitFov))
		{
			float fDistance = CLegitBot::CalcAnglesDistance(pLocal->GetAbsAngles(), angles_to_aim);

			if (fDistance < fClosets)
			{
				fClosets = fDistance;
				iIndex = HITBOX_RIGHT_UPPER_ARM;
			}
		}


	}

	if (bHitboxlegs) {
		Vector left_leg_pos = pEntity->GetHitboxPosition(HITBOX_LEFT_CALF).value();

		QAngle angles_to_aim = M::CalcAngle(local_head_pos, left_leg_pos).Normalize();
		if (EnnemieInFov(pLocal->GetAbsAngles(), angles_to_aim, flLegitFov))
		{
			float fDistance = CLegitBot::CalcAnglesDistance(pLocal->GetAbsAngles(), angles_to_aim);

			if (fDistance < fClosets)
			{
				fClosets = fDistance;
				iIndex = HITBOX_LEFT_CALF;
			}
		}

		Vector right_leg_pos = pEntity->GetHitboxPosition(HITBOX_RIGHT_CALF).value();
		angles_to_aim = M::CalcAngle(local_head_pos, right_leg_pos).Normalize();
		if (EnnemieInFov(pLocal->GetAbsAngles(), angles_to_aim, flLegitFov))
		{
			float fDistance = CLegitBot::CalcAnglesDistance(pLocal->GetAbsAngles(), angles_to_aim);

			if (fDistance < fClosets)
			{
				fClosets = fDistance;
				iIndex = HITBOX_RIGHT_CALF;
			}
		}
	}

	return iIndex;
}

bool CLegitBot::EnnemieInFov(QAngle viewAngles, QAngle anglesToAim, float fov) 
{
	float diffX = CLegitBot::CalcDiffDegrees(viewAngles.x, anglesToAim.x);
	float diffY = CLegitBot::CalcDiffDegrees(viewAngles.y, anglesToAim.y);
	return diffX < fov && diffY < fov;
}

float CLegitBot::CalcAnglesDistance(QAngle angles1, QAngle angles2)
{
	if ((angles2.x == angles1.x) && (angles2.y == angles1.y))
	{
		return 0;
	}
	else
	{
		float theta = angles1.y - angles2.y;
		float dist = (float)(((std::sin(M::Deg2rad(angles1.x))*std::sin(M::Deg2rad(angles2.x)))+(std::cos((M::Deg2rad(angles1.x)))*std::cos((M::Deg2rad(angles2.x)))))*std::cos(M::Deg2rad(theta)));
		dist = (float)std::acos(dist);
		dist = M::Rad2deg(dist);
		dist = (float)(dist * 60 * 1.1515);
		return (dist);
	}
}

float CLegitBot::CalcDiffDegrees(float alpha, float beta)
{
	float phi = (float)std::fmod(std::abs(beta - alpha), 360);
	float distance = phi > 180 ? 360 - phi : phi;
	return distance;
}