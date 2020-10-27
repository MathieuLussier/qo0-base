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

#include "../utilities/math.h"

#include "../utilities/logging.h"

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

	if (pWeapon->GetAmmo() == 0 || !pWeapon->IsWeapon() || pWeaponData->nWeaponType == WEAPONTYPE_KNIFE)
		return;

	// disable when in-menu for more legit (lol)
	if (W::bMainOpened)
		return;

	if (C::Get<bool>(Vars.bLegit))
		CLegitBot::Aimbot(pCmd, pLocal, bSendPacket);
}

void CLegitBot::Aimbot(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket) {

	if (C::Get<int>(Vars.iAimbotKey) > 0 && !IPT::IsKeyDown(C::Get<int>(Vars.iAimbotKey)))
	{
		return;
	}

	int best_ent_index = CLegitBot::GetClosestByCrosshair(pLocal);

	if (best_ent_index > -1) {
		CBaseEntity* ent_toAim = I::ClientEntityList->Get<CBaseEntity>(best_ent_index);

		Vector local_head_pos = pLocal->GetEyePosition();
		//Vector head_pos = ent_toAim->GetHitboxPosition(8).value();
		Vector head_pos = ent_toAim->GetBonePosition(ent_toAim->GetBoneByHash(FNV1A::HashConst("head_0"))).value();

		QAngle angles_to_aim = M::CalcAngle(local_head_pos, head_pos).Normalize();

		I::Engine->SetViewAngles(CLegitBot::Smooth(pLocal->GetAbsAngles(), angles_to_aim);
	}
}

QAngle CLegitBot::Smooth(QAngle angles, QAngle target_angles)
{
	float smooth = C::Get<float>(Vars.flLegitSmooth);
	if (smooth < 1.f)
		return target_angles;
	QAngle delta;
	delta.x = target_angles.x - angles.x;
	delta.y = target_angles.y - angles.y;
	delta.z = 0.f;
	delta.Normalize();
	QAngle new_angle;
	new_angle.x = (angles.x + delta.x) / smooth;
	new_angle.y = (angles.y + delta.y) / smooth;
	new_angle.z = 0.f;
	return new_angle;
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
		Vector head_pos = pEntity->GetBonePosition(pEntity->GetBoneByHash(FNV1A::HashConst("head_0"))).value();

		QAngle angles_to_aim = M::CalcAngle(local_head_pos, head_pos).Normalize();

		if (!EnnemieInFov(pLocal->GetAbsAngles(), angles_to_aim))
			continue;

		float fDistance = CLegitBot::CalcAnglesDistance(pLocal->GetAbsAngles(), angles_to_aim);

		if (fDistance < fClosets)
		{
			fClosets = fDistance;
			iIndex = i;
		}
	}
	return iIndex;
}

bool CLegitBot::EnnemieInFov(QAngle viewAngles, QAngle anglesToAim) 
{
	float fov = C::Get<float>(Vars.flLegitFov);
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
	float phi = std::fmod(std::abs(beta - alpha), 360);
	float distance = phi > 180 ? 360 - phi : phi;
	return distance;
}